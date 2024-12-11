/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanner.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 16:37:27 by alex              #+#    #+#             */
/*   Updated: 2024/12/11 19:21:08 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*find_token_in_list(t_list *start, int letter)
{
	t_list	*current;

	current = start;
	while (current)
	{
		if (((t_token *)current->content)->letter == letter)
			return (current);
		current = current->next;
	}
	return (NULL);
}

void	merge_token(t_cmd_tab *cmd_tab, t_list *start, int condition)
{
	if (condition)
	{
		start->content = ft_strjoinfree(start->content, start->next->content);
		if (!start->content)
			return (set_error(MALLOC_FAIL, cmd_tab, "Failed to malloc token"));
		pop_token(start->next);
	}
}

void	scan(t_cmd_tab *cmd_tab)
{
	cmd_tab->index = 0;
	add_token(cmd_tab, START, '\0');
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
}
