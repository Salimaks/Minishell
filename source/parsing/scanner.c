/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanner.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 16:37:27 by alex              #+#    #+#             */
/*   Updated: 2024/12/02 12:14:06 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	merge_token(t_cmd_tab *cmd_tab, t_token *start, t_token *end)
{
	t_token	*current;

	current = start->next;
	while (current != end->next)
	{
		start->content = ft_strjoinfree(start->content, current->content);
		if (!start->content)
			return (set_error(MALLOC_FAIL, cmd_tab, "Failed to malloc token"));
		current = current->next;
		pop_token(current->prev);
	}
}

void	group_tokens(t_cmd_tab *cmd_tab)
{
	t_token	*current;

	current = cmd_tab->token_list;
	while (current != NULL)
	{
		while (current->lexem == WORD
			&& current->next->lexem == WORD)
			merge_token(cmd_tab, current, current->next);
		while (current->lexem == WHITESPACE
			&& current->next->lexem == WHITESPACE)
			merge_token(cmd_tab, current, current->next);
		current = current->next;
	}
}

void	scan(t_cmd_tab *cmd_tab)
{
	cmd_tab->index = 0;
	while (cmd_tab->index < ft_strlen(cmd_tab->cmd_line))
	{
		if (ft_strchr(DELIMITERS, cmd_tab->cmd_line[cmd_tab->index]))
			add_token(cmd_tab, DELIMITER, cmd_tab->cmd_line[cmd_tab->index]);
		else if (ft_strchr(OPERATORS, cmd_tab->cmd_line[cmd_tab->index]))
			add_token(cmd_tab, OPERATOR, cmd_tab->cmd_line[cmd_tab->index]);
		else if (ft_strchr(WHITESPACES, cmd_tab->cmd_line[cmd_tab->index]))
			add_token(cmd_tab, WHITESPACE, cmd_tab->cmd_line[cmd_tab->index]);
		else
			add_token(cmd_tab, WORD, cmd_tab->cmd_line[cmd_tab->index]);
		cmd_tab->index++;
	}
	add_token(cmd_tab, END, '\0');
	group_tokens(cmd_tab);
}
