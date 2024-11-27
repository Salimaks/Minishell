/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 15:37:36 by mkling            #+#    #+#             */
/*   Updated: 2024/11/27 19:07:58 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"




void	send_fork_exec_cmd(t_cmd_tab *cmd_tab)
{
	t_cmd	*cmd;

	cmd = cmd_tab->cmd_array[cmd_tab->index];
	get_cmd_filepath(cmd_tab);
	execve(cmd->cmd_path, cmd->argv, cmd_tab->env);
	fork_exit_if(1, CANT_EXECUTE_CMD, cmd_tab,
		"Failed command");
}

void	wait_on_fork_index(t_cmd_tab *cmd_tab, int index)
{

	waitpid(cmd_tab->cmd_array, exit_code, 0);
}

int	wait_on_all_forks(t_cmd_tab *cmd_tab)
{
	int	index;

	while (index < cmd_tab->cmd_count)
	{

	}

	exit_code = WEXITSTATUS(exit_code);
	waitpid(-1, 0, WNOHANG);
	if (index->heredoc_flag)
		unlink(HEREDOC_FILEPATH);
	free_index(index);
	return (exit_code);
}

/* */
void	execute_all_cmd(t_cmd_tab *cmd_tab)
{
	cmd_tab->index = 0;
	while (cmd_tab->index < cmd_tab->cmd_count)
	{
		open_pipe(cmd_tab);
		create_forks(cmd_tab);
		connect_pipe(cmd_tab);
		send_fork_exec_cmd(cmd_tab);
		close_pipe(cmd_tab);
		cmd_tab->index++;
	}
	wait_on_all_forks(cmd_tab);
	return (get_last_cmd_exit_code(cmd_tab));
}
