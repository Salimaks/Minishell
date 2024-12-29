/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 17:34:47 by alex              #+#    #+#             */
/*   Updated: 2024/12/29 16:14:49 by mkling           ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "minishell.h"

void	merge_token(t_shell *shell, t_list *start)
{
	t_token	*current;
	t_token	*next;

	current = ((t_token *)start->content);
	next = ((t_token *)start->next->content);
	current->content = ft_strjoinfree(current->content, next->content);
	if (!current->content)
		return (set_error(MALLOC_FAIL, shell, "Failed to malloc token"));
	ft_lstpop(&shell->token_list, start->next, free_token);
}

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
	char	*variable;

	if (shell->critical_er || !token_is(VARIABLE, current) || !current->next)
		return ;
	variable = ft_strjoin("$", ((t_token *)current->next->content)->content);
	((t_token *)current->content)->lexem = VARIABLE;
	((t_token *)current->content)->content = variable;
	ft_lstpop(&shell->token_list, current->next, free_token);
}

void	lexer(t_shell *shell, t_list **token_list)
{
	apply_to_list(shell, *token_list, group_strings);
	apply_to_list(shell, *token_list, remove_space);
	apply_to_list(shell, *token_list, id_variables);
	print_tokens(*token_list);
}

// TO DO
// escape character
// when to expand variables
// id variables

