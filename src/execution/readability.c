/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readability.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 18:37:37 by mkling            #+#    #+#             */
/*   Updated: 2024/12/11 19:51:32 by mkling           ###   ########.fr       */
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

int	get_last_cmd_exit_code(t_cmd_tab *cmd_tab)
{
	return (get_last_cmd(cmd_tab)->exit_code);
}

void	create_fork(t_cmd_tab *cmd_tab)
{
	t_list	*node;
	t_cmd	*cmd;

	if (catch_error(cmd_tab))
		return ;
	node = get_current_cmd_node(cmd_tab);
	cmd = ((t_cmd *)node->content);
	cmd->fork_pid = fork();
	if (cmd->fork_pid == -1)
	{
		perror("Error while forking");
		cmd->exit_code = FORK_ERROR;
	}
}
