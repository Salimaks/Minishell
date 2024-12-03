/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipefork.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 17:41:32 by mkling            #+#    #+#             */
/*   Updated: 2024/12/03 20:09:27 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	fork_exit_if(int condition, int err_code, t_cmd *cmd, char *err_message)
{
	if (condition)
	{
		perror(err_message);
		cmd->exit_code = err_code;
		exit(err_code);
	}
}

void	create_fork(t_cmd_tab *cmd_tab)
{
	t_cmd	*cmd;

	if (catch_error(cmd_tab))
		return ;
	cmd = get_current_cmd(cmd_tab);
	cmd->fork_pid = fork();
	if (cmd->fork_pid == -1)
	{
		perror("Error while forking");
		cmd->exit_code = FORK_ERROR;
	}
}

void	open_pipes(t_cmd_tab *cmd_tab)
{
	t_cmd	*cmd;

	if (catch_error(cmd_tab))
		return ;
	cmd = cmd_tab->cmd_list;
	while (cmd->next != NULL)
	{
		set_error_if(pipe(cmd->pipe_fd) == -1, PIPE_ERROR, cmd_tab,
			"Error while creating pipe");
		cmd = cmd->next;
	}
}

void	close_pipes(t_cmd_tab *cmd_tab)
{
	t_cmd	*cmd;

	cmd = cmd_tab->cmd_list;
	while (cmd->next != NULL)
	{
		close(cmd->pipe_fd[READ]);
		close(cmd->pipe_fd[WRITE]);
		cmd = cmd->next;
	}
}

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




