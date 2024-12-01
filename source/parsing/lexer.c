/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 17:34:47 by alex              #+#    #+#             */
/*   Updated: 2024/12/01 18:35:36 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		is_token_in_list(t_token *start, int character)
{
	t_token	*current;

	current = start;
	while (current)
	{
		if (current->character == character)
			return (1);
		current = current->next;
	}
	return (0);
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
			while (current->lexem != start->lexem
				&& current->lexem != END)
				current = current->next;
			if (current->next->lexem != END)
				current = current->next;
			merge_token(cmd_tab, start, current);
			start->lexem = STRING;
		}
		current = current->next;
	}
}






