/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 17:34:05 by mkling            #+#    #+#             */
/*   Updated: 2024/12/26 13:28:57 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	if (file->fd == -1)
		return (set_cmd_error(OPEN_ERROR, cmd, "Error while opening file"));
}

void	set_infile_fd(t_shell *shell, t_cmd *cmd)
{
	t_file	*file;

	if (cmd->infiles == NULL)
	{
		cmd->fd_in = STDIN_FILENO;
		return ;
	}
	while (cmd->infiles)
	{
		file = (t_file *)cmd->infiles->content;
		if (file->mode == HEREDOC)
			assemble_heredoc(shell, cmd, cmd->infiles);
		if (access(file->path, F_OK) == -1)
			return (set_cmd_error(NO_FILE, cmd, "No input file"));
		if (access(file->path, R_OK) == -1)
			return (set_cmd_error(READ_ERROR, cmd, "Forbidden input file"));
		open_file(file, cmd, READ);
		if (cmd->infiles->next)
			close(file->fd);
		cmd->infiles = cmd->infiles->next;
	}
	cmd->fd_in = file->fd;
}

void	set_outfile_fd(t_cmd *cmd)
{
	t_file	*file;

	if (cmd->outfiles == NULL)
	{
		cmd->fd_out = STDOUT_FILENO;
		return ;
	}
	while (cmd->outfiles)
	{
		file = (t_file *)cmd->outfiles->content;
		if (access(file->path, F_OK) == 0 && access(file->path, W_OK) == -1)
			return (set_cmd_error(READ_ERROR, cmd, "Forbidden output file"));
		if (file->mode == APPEND)
			open_file(file, cmd, APPEND);
		else
			open_file(file, cmd, WRITE);
		if (cmd->outfiles->next)
			close(file->fd);
		cmd->outfiles = cmd->outfiles->next;
	}
	cmd->fd_out = file->fd;
}

void	redirect_io(t_shell *shell, t_cmd *cmd, int input, int output)
{
	if (catch_error(shell))
		return ;
	if (input != STDIN_FILENO && dup2(input, STDIN_FILENO) == -1)
		return (set_cmd_error(DUP_ERROR, cmd, "Error redirecting input"));
	if (output != STDOUT_FILENO && dup2(output, STDOUT_FILENO) == -1)
		return (set_cmd_error(DUP_ERROR, cmd, "Error redirecting output"));
}

void	redirect_for_cmd(t_shell *shell, t_cmd *cmd)
{
	if (catch_error(shell))
		return ;
	set_infile_fd(shell, cmd);
	set_outfile_fd(cmd);
	redirect_io(shell, cmd, cmd->fd_in, cmd->fd_out);
	fprintf(stderr, "redirecting\n");
}
