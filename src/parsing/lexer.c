/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 17:34:47 by alex              #+#    #+#             */
/*   Updated: 2024/12/10 18:52:18 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	group_words(t_cmd_tab *cmd_tab, t_token *token)
{
	while (token->lexem == WORD
		&& token->next->lexem == WORD)
		merge_token(cmd_tab, token, 1);
	while (token->lexem == WHITESPACE
		&& token->next->lexem == WHITESPACE)
		merge_token(cmd_tab, token, 1);
}

void	group_strings(t_cmd_tab *cmd_tab, t_token *start)
{
	t_token	*first_delim;
	t_token	*current;

	if (start->lexem != DELIMITER)
		return ;
	first_delim = start;
	first_delim->content = ft_calloc(1, sizeof(char));
	if (!first_delim->content)
		return (set_error(MALLOC_FAIL, cmd_tab, "Failed to malloc string"));
	current = start->next;
	while (current->lexem != END)
	{
		if (current->letter == first_delim->letter)
			break ;
		merge_token(cmd_tab, start, 1);
		current = current->next;
	}
	first_delim->lexem = STRING;
}

void	remove_space(t_cmd_tab *cmd_tab, t_token *current)
{
	if (catch_error(cmd_tab))
		return ;
	if (current->lexem == WHITESPACE)
	{
		current = current->next;
		pop_token(current->prev);
	}
}

void	id_operators(t_cmd_tab *cmd_tab, t_token *current)
{
	if (catch_error(cmd_tab))
		return ;
	if (current->lexem == OPERATOR)
	{
		current->lexem = current->letter;
		if (current->letter == current->next->letter
			&& current->letter ==  '<')
		{
			current->lexem = HEREDOC;
			pop_token(current->next);
			current->next->lexem = END_OF_HERED;
		}
	}
}

void	lexer(t_cmd_tab *cmd_tab)
{
	scan(cmd_tab);
	apply_to_token_list(cmd_tab, cmd_tab->token_list, group_words);
	apply_to_token_list(cmd_tab, cmd_tab->token_list, group_strings);
	apply_to_token_list(cmd_tab, cmd_tab->token_list, remove_space);
	apply_to_token_list(cmd_tab, cmd_tab->token_list, id_operators);
}

// TO DO
// escape character
// when to expand variables
// id variables

