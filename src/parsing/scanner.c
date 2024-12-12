/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanner.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 16:37:27 by alex              #+#    #+#             */
/*   Updated: 2024/12/12 20:58:13 by mkling           ###   ########.fr       */
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

void	merge_token(t_cmd_tab *cmd_tab, t_list *start)
{
	t_token	*current;
	t_token	*next;

	current = ((t_token *)start->content);
	next = ((t_token *)start->next->content);
	current->content = ft_strjoinfree(current->content, next->content);
	if (!current->content)
		return (set_error(MALLOC_FAIL, cmd_tab, "Failed to malloc token"));
	ft_lstpop(start->next, free_token);
}

void	group_words(t_cmd_tab *cmd_tab, t_list *node)
{
	while (((t_token *)node->content)->lexem == WORD
		&& ((t_token *)node->next->content)->lexem == WORD)
		merge_token(cmd_tab, node);
	while (((t_token *)node->content)->lexem == WHITESPACE
		&& ((t_token *)node->next->content)->lexem == WHITESPACE)
		merge_token(cmd_tab, node);
}

void	group_strings(t_cmd_tab *cmd_tab, t_list *start)
{
	t_token	*first_delim;
	t_list	*current;

	first_delim = ((t_token *)start->content);
	if (first_delim->lexem != DELIMITER)
		return ;
	first_delim->content = ft_calloc(1, sizeof(char));
	if (!first_delim->content)
		return (set_error(MALLOC_FAIL, cmd_tab, "Failed to malloc string"));
	current = start->next;
	while (((t_token *)current->content)->lexem != END)
	{
		if (((t_token *)current->content)->letter == first_delim->letter)
			break ;
		merge_token(cmd_tab, start);
		current = current->next;
	}
	first_delim->lexem = STRING;
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
	apply_to_list(cmd_tab, cmd_tab->token_list, group_words);
	apply_to_list(cmd_tab, cmd_tab->token_list, group_strings);
}
