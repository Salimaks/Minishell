/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 17:34:05 by mkling            #+#    #+#             */
/*   Updated: 2024/11/28 12:23:37 by mkling           ###   ########.fr       */
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
	t_cmd	*current_cmd;

	current_cmd = get_current_cmd(cmd_tab);
	fork_exit_if((access(current_cmd->infile, F_OK) == -1), NO_FILE,
		cmd_tab, "Input file does not exist");
	fork_exit_if((access(current_cmd->infile, R_OK) == -1), READ_ERROR,
		cmd_tab, "Input file cannot be read");
	infile_fd = open_file(current_cmd->infile, READ);
	fork_exit_if((infile_fd == -1), OPEN_ERROR,
		cmd_tab, "Error while opening input file");
	fork_exit_if((redirect(infile_fd, STDIN_FILENO) != OK), DUP_ERROR,
		cmd_tab, "Error while redirecting stdin to infile");
}

void	replace_stdout_by_outfile(t_cmd_tab *cmd_tab)
{
	int		outfile_fd;
	t_cmd	*current_cmd;

	current_cmd = get_current_cmd(cmd_tab);
	fork_exit_if((access(current_cmd->infile, R_OK) == -1), READ_ERROR,
		cmd_tab, "Forbidden input file");
	outfile_fd = open_file(current_cmd->infile, WRITE);
	fork_exit_if((outfile_fd < 0), OPEN_ERROR,
		cmd_tab, "Error while opening output file");
	fork_exit_if((redirect(outfile_fd, STDOUT_FILENO) != OK), DUP_ERROR,
		cmd_tab, "Error while redirecting stdout to outfile");
}

void	replace_stdout_by_pipe_write(t_cmd_tab *cmd_tab)
{
	int		*pipe_fd;
	t_cmd	*current_cmd;

	current_cmd = get_current_cmd(cmd_tab);
	pipe_fd = current_cmd->pipe_fd;
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
