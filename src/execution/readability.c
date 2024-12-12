/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readability.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 18:37:37 by mkling            #+#    #+#             */
/*   Updated: 2024/12/12 20:45:45 by mkling           ###   ########.fr       */
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
