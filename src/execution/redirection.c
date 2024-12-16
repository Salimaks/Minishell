/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 17:34:05 by mkling            #+#    #+#             */
/*   Updated: 2024/12/16 15:49:37 by mkling           ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "minishell.h"

void	open_file(t_file *file, t_cmd *cmd, int mode)
{
	file->fd = 0;
	if (mode == READ)
		file->fd = open(file->path, O_RDONLY);
	if (mode == WRITE)
		file->fd = open(file->path, O_WRONLY | O_TRUNC | O_CREAT, 0666);
	if (mode == APPEND)
		file->fd = open(file->path, O_RDWR | O_APPEND | O_CREAT, 0666);
	fork_exit_if((file->fd == -1), OPEN_ERROR, cmd,
		"Error while opening file");
}

int	get_infile_fd(t_shell *shell, t_cmd *cmd)
{
	t_file	*file;

	if (cmd->infiles == NULL)
		return (STDIN_FILENO);
	while (cmd->infiles)
	{
		file = (t_file *)cmd->infiles->content;
		if (file->mode == HEREDOC)
			assemble_heredoc(shell, cmd, cmd->infiles);
		fork_exit_if(access(file->path, F_OK) == -1, NO_FILE, cmd,
			"Input file does not exist");
		fork_exit_if(access(file->path, R_OK) == -1, READ_ERROR, cmd,
			"Input file cannot be read");
		open_file(file, cmd, READ);
		if (cmd->infiles->next)
			close(file->fd);
		cmd->infiles = cmd->infiles->next;
	}
	return (file->fd);
}

int	get_outfile_fd(t_cmd *cmd)
{
	t_file	*file;

	if (cmd->outfiles == NULL)
		return (STDOUT_FILENO);
	while (cmd->outfiles)
	{
		file = (t_file *)cmd->outfiles->content;
		fork_exit_if(access(file->path, R_OK) == -1,
			READ_ERROR, cmd, "Input file cannot be read");
		if (file->mode == APPEND)
			open_file(file, cmd, APPEND);
		else
			open_file(file, cmd, WRITE);
		if (cmd->outfiles->next)
			close(file->fd);
		cmd->outfiles = cmd->outfiles->next;
	}
	return (file->fd);
}

void	redirect_io(t_shell *shell, t_cmd *cmd, int input, int output)
{
	if (catch_error(shell))
		return ;
	if (input != STDIN_FILENO)
		fork_exit_if((dup2(input, STDIN_FILENO) == -1), DUP_ERROR,
			cmd, "Error while redirecting stdin");
	if (output != STDOUT_FILENO)
		fork_exit_if((dup2(output, STDOUT_FILENO) == -1), DUP_ERROR,
			cmd, "Error while redirecting stdout");
}

void	redirect_fork(t_shell *shell, t_list *node)
{
	t_cmd	*cmd;

	cmd = (t_cmd *)node->content;
	if (catch_error(shell) || cmd->fork_pid != 0)
		return ;
	if (is_first_cmd(node) && is_last_cmd(node))
		redirect_io(shell, cmd,
			get_infile_fd(shell, cmd),
			get_outfile_fd(cmd));
	else if (is_first_cmd(node))
		redirect_io(shell, cmd,
			get_infile_fd(shell, cmd),
			((t_cmd *)node->content)->pipe_fd[WRITE]);
	else if (is_last_cmd(node))
		redirect_io(shell, cmd,
			((t_cmd *)node->prev->content)->pipe_fd[READ],
			get_outfile_fd(cmd));
	else
		redirect_io(shell, cmd,
			((t_cmd *)node->prev->content)->pipe_fd[READ],
			((t_cmd *)node->content)->pipe_fd[WRITE]);
	apply_to_list(shell, shell->cmd_list, close_pipe);
}
