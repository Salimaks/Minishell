/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readability.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 18:37:37 by mkling            #+#    #+#             */
/*   Updated: 2024/12/11 23:52:13 by mkling           ###   ########.fr       */
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

t_list	*get_current_cmd_node(t_cmd_tab *cmd_tab)
{
	t_list	*node;

	node = cmd_tab->cmd_list;
	while (((t_cmd *)node->content)->cmd_index != cmd_tab->index)
		node = node->next;
	return (node);
}

t_cmd	*get_current_cmd(t_cmd_tab *cmd_tab)
{
	t_list	*node;

	node = cmd_tab->cmd_list;
	while (((t_cmd *)node->content)->cmd_index != cmd_tab->index)
		node = node->next;
	return (((t_cmd *)node->content));
}

int	get_last_cmd_exit_code(t_cmd_tab *cmd_tab)
{
	return (((t_cmd *)ft_lstlast(cmd_tab->cmd_list)->content)->exit_code);
}
