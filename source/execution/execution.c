/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 15:37:36 by mkling            #+#    #+#             */
/*   Updated: 2024/11/28 12:54:39 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	send_fork_exec_cmd(t_cmd_tab *cmd_tab)
{
	t_cmd	*cmd;

	cmd = get_current_cmd(cmd_tab);
	if (cmd->fork_pid != 0 || are_error(cmd))
		return ;
	connect_pipe(cmd_tab);
	extract_cmd_filepath(cmd_tab);
	execve(cmd->cmd_path, cmd->argv, cmd_tab->env);
	fork_exit_if(1, CANT_EXECUTE_CMD, cmd,
		"Failed to execute command");
}

/* Reset command table index, wait on all forks with
their forkpid, set their exit code */
void	wait_on_all_forks(t_cmd_tab *cmd_tab)
{
	t_cmd	*cmd;

	cmd_tab->index = 0;
	while (cmd_tab->index < cmd_tab->cmd_count)
	{
		cmd = get_current_cmd(cmd_tab);
		if (!are_error(cmd))
			waitpid(cmd->fork_pid, &cmd->exit_code, 0);
		cmd_tab->index++;
	}
}

/* */
int	execute_all_cmd(t_cmd_tab *cmd_tab)
{
	cmd_tab->index = 0;
	while (cmd_tab->index < cmd_tab->cmd_count)
	{
		create_pipe(cmd_tab);
		create_forks(cmd_tab);
		send_fork_exec_cmd(cmd_tab);
		close_pipe(cmd_tab);
		cmd_tab->index++;
	}
	wait_on_all_forks(cmd_tab);
	return (get_last_cmd_exit_code(cmd_tab));
}

void	free_cmd_tab(t_cmd_tab *cmd_tab)
{
	///
}
