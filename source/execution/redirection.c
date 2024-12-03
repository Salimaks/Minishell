/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 17:34:05 by mkling            #+#    #+#             */
/*   Updated: 2024/12/03 21:33:53 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	open_file(char *filepath, int mode)
{
	int	file_fd;

	file_fd = 0;
	if (mode == READ)
		file_fd = open(filepath, O_RDONLY);
	if (mode == WRITE)
		file_fd = open(filepath, O_WRONLY | O_TRUNC | O_CREAT, 0666);
	if (mode == APPEND)
		file_fd = open(filepath, O_RDWR | O_APPEND | O_CREAT, 0666);
	if (file_fd == -1)
		return (OPEN_ERROR);
	return (file_fd);
}

int	get_infile_fd(t_cmd_tab *cmd_tab)
{
	int		infile_fd;
	t_cmd	*cmd;

	cmd = get_current_cmd(cmd_tab);
	if (cmd->infile == NULL)
		infile_fd = STDIN_FILENO;
	else
	{
		fork_exit_if((access(cmd->infile, F_OK) == -1), NO_FILE,
			cmd, "Input file does not exist");
		fork_exit_if((access(cmd->infile, R_OK) == -1), READ_ERROR,
			cmd, "Input file cannot be read");
		infile_fd = open_file(cmd->infile, READ);
		fork_exit_if((infile_fd == -1), OPEN_ERROR,
			cmd, "Error while opening input file");
	}
	return (infile_fd);
}

int	get_outfile_fd(t_cmd_tab *cmd_tab)
{
	int		outfile_fd;
	t_cmd	*cmd;

	cmd = get_current_cmd(cmd_tab);
	if (cmd->outfile == NULL)
		outfile_fd = STDOUT_FILENO;
	else
	{
		fork_exit_if((access(cmd->infile, R_OK) == -1), READ_ERROR,
			cmd, "Forbidden input file");
		outfile_fd = open_file(cmd->infile, WRITE);
		fork_exit_if((outfile_fd < 0), OPEN_ERROR,
			cmd, "Error while opening output file");
	}
	return (outfile_fd);
}

void	redirect_in_and_out(t_cmd_tab *cmd_tab, int input, int output)
{
	t_cmd	*cmd;

	cmd = get_current_cmd(cmd_tab);
	fork_exit_if((dup2(input, STDIN_FILENO) == -1), DUP_ERROR,
		cmd, "Error while redirecting stdin");
	fork_exit_if((dup2(output, STDOUT_FILENO) == -1), DUP_ERROR,
		cmd, "Error while redirecting stdout");
}

void	connect_pipe(t_cmd_tab *cmd_tab)
{
	t_cmd	*cmd;

	cmd = get_current_cmd(cmd_tab);
	if (is_first_cmd(cmd_tab))
		redirect_in_and_out(cmd_tab, get_infile_fd(cmd_tab), cmd->pipe_fd[WRITE]);
	else if (is_last_cmd(cmd_tab))
		redirect_in_and_out(cmd_tab, cmd->prev->pipe_fd[READ], get_outfile_fd(cmd_tab));
	else
		redirect_in_and_out(cmd_tab, cmd->prev->pipe_fd[READ], cmd->pipe_fd[WRITE]);
	close_pipes(cmd_tab);
}
