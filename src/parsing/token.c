/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 18:04:03 by alex              #+#    #+#             */
/*   Updated: 2024/12/27 19:01:56 by mkling           ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "minishell.h"

t_token	*get_last_token(t_shell *shell)
{
	return ((t_token *)ft_lstlast(shell->token_list)->content);
}

t_token	*create_token(t_shell *shell, char letter, char *content)
{
	t_token	*token;

	token = (t_token *)ft_calloc(sizeof(t_token), 1);
	if (!token)
		return (set_error(MALLOC_FAIL, shell, "Failed to alloc token"), NULL);
	token->letter = letter;
	if (ft_strchr(OPERATORS, letter))
		token->lexem = letter;
	else if (ft_strchr(BLANKS, letter))
		token->lexem = BLANK;
	else if (ft_strchr(DELIMITERS, letter))
		token->lexem = DELIMITER;
	else if (letter == END || letter == START)
		token->lexem = letter;
	else if (content)
		token->content = content;
	return (token);
}

void	add_token(t_shell *shell, t_list **dest, char letter, char *content)
{
	t_token	*token;

	token = create_token(shell, letter, content);
	ft_lstadd_back(dest, ft_lstnew(token));
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
