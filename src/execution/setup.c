/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 12:37:12 by alex              #+#    #+#             */
/*   Updated: 2024/12/11 19:45:09 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*get_last_cmd(t_cmd_tab *cmd_tab)
{
	return ((t_cmd *)ft_lstlast(cmd_tab->cmd_list)->content);
}

t_cmd	*create_cmd(t_cmd_tab *cmd_tab)
{
	t_cmd	*cmd;
	t_list	*node;

	cmd = (t_cmd *)ft_calloc(sizeof(t_cmd), 1);
	if (!cmd)
		return (NULL);
	cmd->exit_code = 0;
	cmd->fork_pid = -1;
	cmd->argv = NULL;
	node = ft_lstnew(cmd);
	ft_lstadd_front(cmd_tab->cmd_list, node);
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
