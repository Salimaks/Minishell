/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skassimi <skassimi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 18:04:03 by alex              #+#    #+#             */
/*   Updated: 2025/01/09 14:23:53 by skassimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*get_last_token(t_shell *shell)
{
	return ((t_token *)ft_lstlast(shell->token_list)->content);
}

void	add_token(t_shell *shell, int lexem, char letter)
{
	t_token	*token;
	t_list	*node;

	token = (t_token *)ft_calloc(sizeof(t_token), 1);
	if (!token)
		return (set_error(MALLOC_FAIL, shell, "Failed to allocate token"));
	token->content = ft_calloc(sizeof(char), 2);
	if (!token->content)
		return (set_error(MALLOC_FAIL, shell, "Failed to alloc token data"));
	token->lexem = lexem;
	token->letter = letter;
	token->content[0] = letter;
	token->content[1] = '\0';
	node = ft_lstnew(token);
	if (!node)
		return (set_error(MALLOC_FAIL, shell, "Failed to alloc token node"));
	ft_lstadd_back(&shell->token_list, node, token->lexem);
}

void	apply_to_list(t_shell *shell, t_list *node,
			void function(t_shell *, t_list *))
{
	while (node != NULL)
	{
		function(shell, node);
		node = node->next;
	}
}
