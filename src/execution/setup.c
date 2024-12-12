/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 12:37:12 by alex              #+#    #+#             */
/*   Updated: 2024/12/12 20:45:42 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	increment_all_cmd_index(t_list *start)
{
	t_list	*current;
	t_cmd	*curr_cmd;

	current = start;
	while (current != NULL)
	{
		curr_cmd = ((t_cmd *)current->content);
		curr_cmd->cmd_index++;
		current = current->next;
	}
}

t_cmd	*create_cmd(t_cmd_tab *cmd_tab)
{
	t_cmd	*cmd;
	t_list	*node;

	cmd = (t_cmd *)ft_calloc(sizeof(t_cmd), 1);
	if (!cmd)
		return (NULL);
	cmd->fork_pid = -1;
	node = ft_lstnew(cmd);
	ft_lstadd_front(&cmd_tab->cmd_list, node);
	increment_all_cmd_index(cmd_tab->cmd_list->next);
	return (cmd);
}

void	create_file(t_cmd_tab *cmd_tab, t_cmd *cmd, t_token *token)
{
	t_file	*file;
	t_list	*node;

	file = (t_file *)ft_calloc(sizeof(t_file), 1);
	if (!file)
		return (set_error(MALLOC_FAIL, cmd_tab, "Failed to allocate filename"));
	file->mode = token->lexem;
	file->path = ft_strdup(token->content);
	if (!file->path)
		return (set_error(MALLOC_FAIL, cmd_tab, "Failed to allocate filename"));
	node = ft_lstnew(file);
	if (!node)
		return (set_error(MALLOC_FAIL, cmd_tab, "Failed to alloc file node"));
	if (file->mode == HEREDOC)
	{
		file->delim = file->path;
		file->path = generate_heredoc_filepath(cmd_tab);
		if (!file->path)
			return (set_error(MALLOC_FAIL, cmd_tab, "Failed hered generation"));
	}
	if (file->mode == INFILE || file->mode == HEREDOC)
		ft_lstadd_back(&cmd->infiles, node);
	if (file->mode == OUTFILE || file->mode == APPEND)
		ft_lstadd_back(&cmd->outfiles, node);
}

t_cmd_tab	*create_cmd_tab(char **env)
{
	t_cmd_tab	*cmd_tab;

	cmd_tab = ft_calloc(sizeof(t_cmd_tab), 1);
	if (!cmd_tab)
		return (NULL);
	cmd_tab->env = env;
	extract_paths(cmd_tab);
	extract_env_as_linked_list(cmd_tab);
	return (cmd_tab);
}
