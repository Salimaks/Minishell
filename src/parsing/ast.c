/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 21:51:24 by mkling            #+#    #+#             */
/*   Updated: 2024/12/15 18:28:15 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ast	*create_ast_node(t_shell *shell, int type, void *content)
{
	t_ast	*node;

	node = ft_calloc(1, sizeof(t_ast));
	if (!node)
		return (set_error(MALLOC_FAIL, shell, "Failed to malloc ast"), NULL);
	node->type = type;
	node->content = content;
	node->left = NULL;
	node->left = NULL;
	return (node);
}

void	process_ast(t_shell *shell, t_ast *ast)
{
	if (ast->type == AST_CMD)
	{
		ft_lstadd_back(&shell->cmd_list, ft_lstnew(ast->content));
		ast->content = NULL;
	}
	if (ast->type == AST_PIPE)
	{
		process_ast(shell, ast->left);
		process_ast(shell, ast->right);
	}
}

