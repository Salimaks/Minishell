/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 17:34:47 by alex              #+#    #+#             */
/*   Updated: 2024/12/03 08:25:22 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*find_token_in_list(t_token *start, int letter)
{
	t_token	*current;

	current = start;
	while (current)
	{
		if (current->letter == letter)
			return (current);
		current = current->next;
	}
	return (NULL);
}

void	group_strings(t_cmd_tab *cmd_tab)
{
	t_token	*current;
	t_token	*start;

	current = cmd_tab->token_list;
	while (current != NULL)
	{
		if (current->lexem == DELIMITER)
		{
			start = current;
			current = current->next;
			while (current->letter != start->letter
				&& current->lexem != END)
			{
				merge_token(cmd_tab, start, current);
				current = current->next;
			}
			if (current->lexem != END)
				merge_token(cmd_tab, start, current);
			start->lexem = STRING;
		}
		current = current->next;
	}
}

void	remove_space(t_cmd_tab *cmd_tab)
{
	t_token	*current;

	current = cmd_tab->token_list;
	while (current != NULL)
	{
		if (current->lexem == WHITESPACE)
		{
			current = current->next;
			pop_token(current->prev);
		}
		else
			current = current->next;
	}
}

void	lexer(t_cmd_tab *cmd_tab)
{
	scan(cmd_tab);
	group_strings(cmd_tab);
	remove_space(cmd_tab);
}






