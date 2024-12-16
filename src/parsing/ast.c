/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 21:51:24 by mkling            #+#    #+#             */
/*   Updated: 2024/12/16 16:17:44 by mkling           ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

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
	if (ast == NULL)
		return (set_error(SYNTAX_ERROR, shell, "Empty ast"));
	if (ast->type == AST_CMD)
	{
		ft_lstadd_back(&shell->cmd_list, ft_lstnew(ast->content));
		ast->content = NULL;
		return ;
	}
	if (ast->type == AST_PIPE)
	{
		process_ast(shell, ast->left);
		process_ast(shell, ast->right);
	}
}

