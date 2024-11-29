/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readability.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 18:37:37 by mkling            #+#    #+#             */
/*   Updated: 2024/11/29 13:24:57 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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

/* Protection aganst any critical error
such as a malloc, fork or pipe failure */
int	are_error(t_cmd_tab *cmd_tab)
{
	if (cmd_tab->critical_er)
		return (cmd_tab->critical_er);
	if (get_current_cmd(cmd_tab)->exit_code)
		return (1);
	return (0);
}
