/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readability.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 18:37:37 by mkling            #+#    #+#             */
/*   Updated: 2024/11/29 16:46:25 by alex             ###   ########.fr       */
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
