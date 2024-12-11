/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 17:34:47 by alex              #+#    #+#             */
/*   Updated: 2024/12/11 22:26:36 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	remove_space(t_cmd_tab *cmd_tab, t_list *current)
{
	if (catch_error(cmd_tab))
		return ;
	if (((t_token *)current->content)->lexem == WHITESPACE)
	{
		current = current->next;
		ft_lstpop(current->prev, free_token);
	}
}

void	id_redirections(t_cmd_tab *cmd_tab, t_list *current)
{
	if (catch_error(cmd_tab))
		return ;
	if (((t_token *)current->content)->letter == '<'
		&& ((t_token *)current->next->content)->letter == '<')
	{
		current = current->next->next;
		((t_token *)current->content)->lexem = HEREDOC;
		ft_lstpop(current->prev, free_token);
		ft_lstpop(current->prev, free_token);
	}
	else if (((t_token *)current->content)->letter == '<')
	{
		current = current->next;
		((t_token *)current->content)->lexem = INFILE;
		ft_lstpop(current->prev, free_token);
	}
	else if (((t_token *)current->content)->letter == '>')
	{
		current = current->next;
		((t_token *)current->content)->lexem = OUTFILE;
		ft_lstpop(current->prev, free_token);
	}
}

void	lexer(t_cmd_tab *cmd_tab)
{
	scan(cmd_tab);
	apply_to_list(cmd_tab, cmd_tab->token_list, group_words);
	apply_to_list(cmd_tab, cmd_tab->token_list, group_strings);
	apply_to_list(cmd_tab, cmd_tab->token_list, remove_space);
	apply_to_list(cmd_tab, cmd_tab->token_list, id_redirections);
}

// TO DO
// escape character
// when to expand variables
// id variables

