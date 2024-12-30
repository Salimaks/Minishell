/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 17:34:47 by alex              #+#    #+#             */
/*   Updated: 2024/12/30 13:25:59 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	remove_space(t_shell *shell, t_list *current)
{
	if (shell->critical_er || !token_is(BLANK, current->next))
		return ;
	ft_lstpop(&shell->token_list, current->next, free_token);
}

void	group_strings(t_shell *shell, t_list *node)
{
	t_token	*first_delim;

	first_delim = ((t_token *)node->content);
	if (first_delim->lexem != DELIMITER)
		return ;
	first_delim->content = ft_calloc(1, sizeof(char));
	if (!first_delim->content)
		return (set_error(MALLOC_FAIL, shell, "Failed to malloc string"));
	while (!token_is(END, node->next))
	{
		if (((t_token *)node->next->content)->letter == first_delim->letter)
		{
			ft_lstpop(&shell->token_list, node->next, free_token);
			break ;
		}
		merge_token(shell, node);
	}
	first_delim->lexem = STRING;
}

void	id_variables(t_shell *shell, t_list *current)
{
	if (shell->critical_er || ((t_token *)current->content)->lexem != OPERATOR
		|| ((t_token *)current->content)->letter != '$')
		return ;
	((t_token *)current->content)->lexem = VARIABLE;
	((t_token *)current->content)->content
		= ft_strjoin("$", (char *)((t_token *)current->next->content)->content);
	ft_lstpop(&shell->token_list, current->next, free_token);
}

void	lexer(t_shell *shell, t_list **token_list)
{
	apply_to_list(shell, *token_list, id_variables);
	apply_to_list(shell, *token_list, group_strings);
	apply_to_list(shell, *token_list, remove_space);
	// print_tokens(*token_list);
}

// TO DO
// escape character
// when to expand variables
// id variables

