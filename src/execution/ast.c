/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 21:51:24 by mkling            #+#    #+#             */
/*   Updated: 2024/12/26 20:49:29 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_tree	*create_branch(t_shell *shell, int type, void *content)
{
	t_tree	*node;

	node = ft_calloc(1, sizeof(t_tree));
	if (!node)
		return (set_error(MALLOC_FAIL, shell, "Failed to malloc tree"), NULL);
	node->type = type;
	node->content = content;
	node->left = NULL;
	node->left = NULL;
	return (node);
}

void	put_arg_in_array(t_cmd *cmd)
{
	int	argc;
	int	index;

	argc = ft_lstsize(cmd->arg_list);
	cmd->argv = ft_calloc(sizeof(char *), argc + 1);
	if (!cmd->argv)
		return (set_cmd_error(MALLOC_FAIL, cmd, "Failed to malloc argv"));
	index = 0;
	while (index < argc)
	{
		cmd->argv[index] = cmd->arg_list->content;
		cmd->arg_list = cmd->arg_list->next;
		index++;
	}
}
