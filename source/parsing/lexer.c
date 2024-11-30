/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 17:34:47 by alex              #+#    #+#             */
/*   Updated: 2024/11/30 22:02:08 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	scan(t_cmd_tab *cmd_tab)
{
	cmd_tab->index = 0;
	while (cmd_tab->index < ft_strlen(cmd_tab->cmd_line))
	{
		if (ft_strchr(SPECIALS, cmd_tab->cmd_line[cmd_tab->index]))
			add_token(cmd_tab, SPECIAL, cmd_tab->cmd_line[cmd_tab->index]);
		else if (ft_strchr(DELIMITERS, cmd_tab->cmd_line[cmd_tab->index]))
			add_token(cmd_tab, DELIMITER, cmd_tab->cmd_line[cmd_tab->index]);
		else if (ft_strchr(OPERATORS, cmd_tab->cmd_line[cmd_tab->index]))
			add_token(cmd_tab, OPERATOR, cmd_tab->cmd_line[cmd_tab->index]);
		else if (ft_strchr(WHITESPACE, cmd_tab->cmd_line[cmd_tab->index]))
			add_token(cmd_tab, SPACES, cmd_tab->cmd_line[cmd_tab->index]);
		else
			add_token(cmd_tab, LETTER, cmd_tab->cmd_line[cmd_tab->index]);
		cmd_tab->index++;
	}
	add_token(cmd_tab, END, '\0');
}

int		get_num_token_between(t_token *start, t_token *end)
{
	t_token	*current;
	int		len;

	current = start;
	len = 1;
	while (current != end)
	{
		len++;
		current = current->next;
	}
	return (len);
}

void	group_token(t_cmd_tab *cmd_tab, t_token *start, t_token *end)
{
	t_token	*current;
	char	*content;
	int		len;
	int		index;

	len = get_num_token_between(start, end);
	content = (char *)malloc(sizeof(char) * (len + 1));
	if (!content)
		return (set_error(MALLOC_FAIL, cmd_tab, "Failed to malloc token content"));
	current = start;
	index = 0;
	while (index < len)
	{
		content[index] = current->character;
		current = current->next;
		if (index > 0)
			pop_token(current->prev);
		index++;
	}
	content[index] = '\0';
	start->content = content;
}

void	group_same_type_token(t_cmd_tab *cmd_tab, t_token *start)
{
	t_token	*end;

	end = start;
	while (end->next->lexem == start->lexem)
		end = end->next;
	group_token(cmd_tab, start, end);
}

void	tokenize(t_cmd_tab *cmd_tab)
{
	t_token	*current;

	scan(cmd_tab);
	current = cmd_tab->token_list;
	while (current != NULL)
	{
		if (current->lexem == LETTER)
			group_same_type_token(cmd_tab, current);
		else if (current->lexem == SPACES)
			group_same_type_token(cmd_tab, current);
		else if (current->lexem == ESCAPE)
			group_escape(cmd_tab, current);
		current = current->next;
	}
}
