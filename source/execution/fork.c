/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 17:41:32 by mkling            #+#    #+#             */
/*   Updated: 2024/11/28 12:51:52 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/minishell.h"

void	fork_exit_if(int condition, int err_code, t_cmd *cmd, char *err_message)
{
	if (condition)
	{
		ft_putstr_fd(err_message);
		ft_putstr_fd("\n");
		cmd->exit_code = err_code;
		exit(err_code);
	}
}

void	create_fork(t_cmd_tab *cmd_tab)
{
	t_cmd	*cmd;

	if (critical_error(cmd_tab))
		return ;
	cmd = get_current_cmd(cmd_tab);
	cmd->fork_pid = fork();
	if (cmd->fork_pid == -1)
	{
		perror("Error while forking");
		cmd->exit_code = FORK_ERROR;
	}
}

void	create_pipe(t_cmd_tab *cmd_tab)
{
	t_cmd	*cmd;

	cmd = get_current_cmd(cmd_tab);
	if (is_last_cmd(cmd_tab) | are_error(cmd_tab))
		return ;
	if (pipe(cmd->pipe_fd) == -1)
	{
		perror("Error while creating pipe");
		cmd->exit_code = PIPE_ERROR;
	}
}

void	close_pipe(t_cmd_tab *cmd_tab)
{
	t_cmd	*cmd;

	cmd = get_current_cmd(cmd_tab);
	if (is_last_cmd(cmd_tab))
		return ;
	close(cmd->pipe_fd[WRITE]);
	if (dup2(cmd->pipe_fd[READ], STDIN_FILENO) == -1)
	{
		perror("Error while redirecting pipe to stdin");
		cmd->exit_code = DUP_ERROR;
	}
	close(cmd->pipe_fd[READ]);
	return (OK);
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




