/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readability.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 18:37:37 by mkling            #+#    #+#             */
/*   Updated: 2024/11/28 12:49:53 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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
	return (cmd_tab->cmd_array[cmd_tab->index]);
}

int	get_last_cmd_exit_code(t_cmd_tab *cmd_tab)
{
	return (cmd_tab->cmd_array[cmd_tab->cmd_count - 1]->exit_code);
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
