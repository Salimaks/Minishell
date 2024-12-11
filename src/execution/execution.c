/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 15:37:36 by mkling            #+#    #+#             */
/*   Updated: 2024/12/11 10:40:38 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*open all required pipes*/
void	open_pipes(t_cmd_tab *cmd_tab)
{
	t_cmd	*cmd;

	if (catch_error(cmd_tab) || cmd_tab->cmd_count <= 1)
		return ;
	cmd = cmd_tab->cmd_list;
	while (cmd->next != NULL)
	{
		set_error_if(pipe(cmd->pipe_fd) == -1, PIPE_ERROR, cmd_tab,
			"Error while creating pipe");
		cmd = cmd->next;
	}
}

/* If fork,
Redirect stdout and stdin,
Check absolute then relative command path,
Sends fork to execve the command
Sets error if execve fails */
void	send_fork_exec_cmd(t_cmd_tab *cmd_tab)
{
	t_cmd	*cmd;

	cmd = get_current_cmd(cmd_tab);
	if (cmd->fork_pid != 0 || catch_error(cmd_tab))
		return ;
	connect_pipe(cmd_tab);
	get_cmd_path(cmd, cmd_tab);
	execve(cmd->cmd_path, cmd->argv, cmd_tab->env);
	fork_exit_if(1, CANT_EXECUTE_CMD, cmd,
		"Failed to execute command");
}

/*closes all opened pipes*/
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

/* Reset command table index,
Wait on all forks with their forkpid,
Set their exit code */
void	wait_on_all_forks(t_cmd_tab *cmd_tab)
{
	t_cmd	*cmd;

	cmd_tab->index = 0;
	while (cmd_tab->index < cmd_tab->cmd_count)
	{
		cmd = get_current_cmd(cmd_tab);
		if (!catch_error(cmd_tab))
			waitpid(cmd->fork_pid, &cmd->exit_code, 0);
		cmd_tab->index++;
	}
}

int	execute_all_cmd(t_cmd_tab *cmd_tab)
{
	cmd_tab->index = 0;
	open_pipes(cmd_tab);
	while (cmd_tab->index < cmd_tab->cmd_count)
	{
		create_fork(cmd_tab);
		send_fork_exec_cmd(cmd_tab);
		cmd_tab->index++;
	}
	close_pipes(cmd_tab);
	wait_on_all_forks(cmd_tab);
	return (get_last_cmd_exit_code(cmd_tab));
}
