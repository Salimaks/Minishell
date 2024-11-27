/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 17:34:05 by mkling            #+#    #+#             */
/*   Updated: 2024/11/27 18:50:40 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	redirect(int new_fd, int old_fd)
{
	if (new_fd < 0 || old_fd < 0)
		return (INVALID_FD);
	if (dup2(new_fd, old_fd == -1))
	{
		close(new_fd);
		return (DUP_ERROR);
	}
	close(new_fd);
	return (OK);
}

void	replace_stdin_by_infile(t_cmd_tab *cmd_tab)
{
	int		infile_fd;
	char	*infile_filepath;
	t_cmd	*current_cmd;

	current_cmd = cmd_tab->cmd_array[cmd_tab->index];
	infile_filepath = current_cmd->infile;
	fork_exit_if((access(infile_filepath, F_OK) == -1), NO_FILE,
		cmd_tab, "Input file does not exist");
	fork_exit_if((access(infile_filepath, R_OK) == -1), READ_ERROR,
		cmd_tab, "Input file cannot be read");
	infile_fd = open_file(infile_filepath, READ);
	fork_exit_if((infile_fd == -1), OPEN_ERROR,
		cmd_tab, "Error while opening input file");
	fork_exit_if((redirect(infile_fd, STDIN_FILENO) != OK), DUP_ERROR,
		cmd_tab, "Error while redirecting stdin to infile");
}

void	replace_stdout_by_outfile(t_cmd_tab *cmd_tab)
{
	int		outfile_fd;
	char	*outfile_filepath;
	t_cmd	*current_cmd;

	current_cmd = cmd_tab->cmd_array[cmd_tab->index];
	outfile_filepath = current_cmd->infile;
	fork_exit_if((access(outfile_filepath, R_OK) == -1), READ_ERROR,
		cmd_tab, "Forbidden input file");
	outfile_fd = open_file(outfile_filepath, WRITE);
	fork_exit_if((outfile_fd < 0), OPEN_ERROR,
		cmd_tab, "Error while opening output file");
	fork_exit_if((redirect(outfile_fd, STDOUT_FILENO) != OK), DUP_ERROR,
		cmd_tab, "Error while redirecting stdout to outfile");
}

void	replace_stdout_by_pipe_write(t_cmd_tab *cmd_tab)
{
	int		*pipe_fd[2];
	t_cmd	*current_cmd;

	current_cmd = cmd_tab->cmd_array[cmd_tab->index];
	pipe_fd = cmd_tab->pipefd[cmd_tab->index];
	fork_exit_if((redirect(pipe_fd[WRITE], STDOUT_FILENO) == -1), DUP_ERROR,
		cmd_tab, "Error while redirecting stdout to pipe");
	close(pipe_fd[READ]);
}

void	connect_pipe(t_cmd_tab *cmd_tab)
{
	if (is_first_cmd(cmd_tab))
		replace_stdin_by_infile(cmd_tab);
	if (is_last_cmd(cmd_tab))
		replace_stdout_by_outfile(cmd_tab);
	else
		replace_stdout_by_pipe_write(cmd_tab);
}
