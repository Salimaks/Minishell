/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 17:34:05 by mkling            #+#    #+#             */
/*   Updated: 2024/12/11 23:56:44 by mkling           ###   ########.fr       */
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
	int		index;
	t_cmd	*cmd;
	t_file	*file;

	cmd = get_current_cmd(cmd_tab);
	if (cmd->infiles == NULL)
		return (STDIN_FILENO);
	index = 0;
	while (cmd->infiles)
	{
		file = (t_file *)cmd->infiles->content;
		fork_exit_if(access(file->path, F_OK) == -1, NO_FILE, cmd,
			"Input file does not exist");
		fork_exit_if(access(file->path, R_OK) == -1, READ_ERROR, cmd,
			"Input file cannot be read");
		file->fd = open_file(file->path, READ);
		fork_exit_if((file->fd == -1), OPEN_ERROR, cmd,
			"Error while opening input file");
		if (cmd->infiles->next)
			close(file->fd);
		cmd->infiles = cmd->infiles->next;
	}
	return (file->fd);
}

int	get_outfile_fd(t_cmd_tab *cmd_tab)
{
	int		index;
	t_cmd	*cmd;
	t_file	*file;

	cmd = get_current_cmd(cmd_tab);
	if (cmd->outfiles == NULL)
		return (STDOUT_FILENO);
	index = 0;
	while (cmd->outfiles)
	{
		file = (t_file *)cmd->outfiles->content;
		file->fd = open_file(file->path, WRITE);
		fork_exit_if((file->fd < 0), OPEN_ERROR,
			cmd, "Error while opening output file");
		if (cmd->outfiles->next)
			close(file->fd);
		cmd->outfiles = cmd->outfiles->next;
	}
	return (file->fd);
}

void	redirect_in_and_out(t_cmd_tab *cmd_tab, int input, int output)
{
	t_cmd	*cmd;

	cmd = get_current_cmd(cmd_tab);
	fprintf(stderr, "input is %d, output is %d\n", input, output);
	fork_exit_if((dup2(input, STDIN_FILENO) == -1), DUP_ERROR,
		cmd, "Error while redirecting stdin");
	fork_exit_if((dup2(output, STDOUT_FILENO) == -1), DUP_ERROR,
		cmd, "Error while redirecting stdout");
	fprintf(stderr, "input is %d, output is %d\n", input, output);
}

void	connect_pipe(t_cmd_tab *cmd_tab)
{
	t_list	*node;

	node = get_current_cmd_node(cmd_tab);
	if (is_first_cmd(cmd_tab) && is_last_cmd(cmd_tab))
		redirect_in_and_out(cmd_tab,
			get_infile_fd(cmd_tab),
			get_outfile_fd(cmd_tab));
	else if (is_first_cmd(cmd_tab))
		redirect_in_and_out(cmd_tab,
			get_infile_fd(cmd_tab),
			((t_cmd *)node->content)->pipe_fd[WRITE]);
	else if (is_last_cmd(cmd_tab))
		redirect_in_and_out(cmd_tab,
			((t_cmd *)node->prev->content)->pipe_fd[READ],
			get_outfile_fd(cmd_tab));
	else
		redirect_in_and_out(cmd_tab,
			((t_cmd *)node->prev->content)->pipe_fd[READ],
			((t_cmd *)node->content)->pipe_fd[WRITE]);
	close_pipes(cmd_tab);
}
