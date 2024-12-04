/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 12:37:12 by alex              #+#    #+#             */
/*   Updated: 2024/12/03 22:21:18 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*get_last_cmd(t_cmd_tab *cmd_tab)
{
	t_cmd	*current;

	if (cmd_tab == NULL)
		return (NULL);
	current = cmd_tab->cmd_list;
	while (current->next != NULL)
		current = current->next;
	return (current);
}

void	increment_all_cmd_index(t_cmd *start)
{
	t_cmd	*current;

	current = start;
	while (current != NULL)
	{
		current->cmd_index++;
		current = current->next;
	}
}

void	append_cmd(t_cmd *cmd, t_cmd_tab *cmd_tab)
{
	cmd->cmd_index = 0;
	if (cmd_tab->cmd_list == NULL)
		cmd_tab->cmd_list = cmd;
	else
	{
		cmd_tab->cmd_list->prev = cmd;
		cmd->next = cmd_tab->cmd_list;
		cmd_tab->cmd_list = cmd;
		increment_all_cmd_index(cmd->next);
	}
}

t_cmd	*create_cmd(t_cmd_tab *cmd_tab)
{
	t_cmd	*cmd;

	cmd = (t_cmd *)ft_calloc(sizeof(t_cmd), 1);
	if (!cmd)
		return (NULL);
	cmd->prev = NULL;
	cmd->next = NULL;
	cmd->exit_code = 0;
	cmd->fork_pid = -1;
	cmd->argv = NULL;
	append_cmd(cmd, cmd_tab);
	return (cmd);
}

t_cmd_tab	*create_cmd_tab(char **env)
{
	t_cmd_tab	*cmd_tab;

	cmd_tab = ft_calloc(sizeof(t_cmd_tab), 1);
	if (!cmd_tab)
		return (NULL);
	cmd_tab->index = 0;
	cmd_tab->env = env;
	extract_paths(cmd_tab);
	return (cmd_tab);
}
