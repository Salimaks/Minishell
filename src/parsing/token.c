/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 18:04:03 by alex              #+#    #+#             */
/*   Updated: 2024/12/11 19:28:53 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*get_last_token(t_cmd_tab *cmd_tab)
{
	return ((t_token *)ft_lstlast(cmd_tab->token_list)->content);
}

void	add_token(t_cmd_tab *cmd_tab, int lexem, char letter)
{
	t_token	*token;
	t_list	*node;

	token = (t_token *)ft_calloc(sizeof(t_token), 1);
	if (!token)
		return (set_error(MALLOC_FAIL, cmd_tab,
				"Failed to allocate token"));
	token->content = ft_calloc(sizeof(char), 2);
	if (!token->content)
		return (set_error(MALLOC_FAIL, cmd_tab,
				"Failed to allocate token content"));
	token->lexem = lexem;
	token->letter = letter;
	token->content[0] = letter;
	token->content[1] = '\0';
	node = ft_lstnew(token);
	if (!node)
		return (set_error(MALLOC_FAIL, cmd_tab,
				"Failed to allocate token node"));
	ft_lstadd_back(cmd_tab->token_list, node);
}

void	apply_to_token_list(t_cmd_tab *cmd_tab, t_list *node,
			void function(t_cmd_tab*, t_token*))
{
	t_list	*current;

	current = node;
	while (current != NULL)
	{
		function(cmd_tab, current->content);
		current = current->next;
	}
}
