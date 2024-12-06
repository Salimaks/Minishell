/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanner.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akling <akling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 16:37:27 by alex              #+#    #+#             */
/*   Updated: 2024/12/06 14:13:19 by akling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	merge_token(t_cmd_tab *cmd_tab, t_token *start, int condition)
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
