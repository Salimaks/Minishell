/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readability.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 18:37:37 by mkling            #+#    #+#             */
/*   Updated: 2024/12/03 21:17:30 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_last_cmd(t_cmd_tab *cmd_tab)
{
	return (cmd_tab->index == cmd_tab->cmd_count - 1);
}

int	is_first_cmd(t_cmd_tab *cmd_tab)
{
	return (cmd_tab->index == 0);
}

t_cmd	*get_current_cmd(t_cmd_tab *cmd_tab)
{
	t_cmd	*cmd;

	cmd = cmd_tab->cmd_list;
	while (cmd->cmd_index != cmd_tab->index)
		cmd = cmd->next;
	return (cmd);
}

int	get_last_cmd_exit_code(t_cmd_tab *cmd_tab)
{
	return (get_last_cmd(cmd_tab)->exit_code);
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
