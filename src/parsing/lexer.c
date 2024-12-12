/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 17:34:47 by alex              #+#    #+#             */
/*   Updated: 2024/12/13 00:21:43 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	remove_space(t_cmd_tab *cmd_tab, t_list *current)
{
	if (catch_error(cmd_tab)
		|| ((t_token *)current->content)->lexem != WHITESPACE)
		return ;
	current = current->next;
	ft_lstpop(current->prev, free_token);
}

void	id_heredoc(t_cmd_tab *cmd_tab, t_list *current)
{
	if (catch_error(cmd_tab)
		|| ((t_token *)current->content)->lexem != OPERATOR)
		return ;
	if (((t_token *)current->content)->letter == '<'
		&& ((t_token *)current->next->content)->letter == '<')
	{
		current = current->next->next;
		((t_token *)current->content)->lexem = HEREDOC;
		ft_lstpop(current->prev, free_token);
		ft_lstpop(current->prev, free_token);
	}
	else if (((t_token *)current->content)->letter == '>'
		&& ((t_token *)current->next->content)->letter == '>')
	{
		current = current->next->next;
		((t_token *)current->content)->lexem = APPEND;
		ft_lstpop(current->prev, free_token);
		ft_lstpop(current->prev, free_token);
	}
}

void	id_redirections(t_cmd_tab *cmd_tab, t_list *current)
{
	if (catch_error(cmd_tab)
		|| ((t_token *)current->content)->lexem != OPERATOR)
		return ;
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

void	id_variables(t_cmd_tab * cmd_tab, t_list *current)
{
	if (catch_error(cmd_tab)
		|| ((t_token *)current->content)->lexem != OPERATOR)
		return ;
	if (((t_token *)current->content)->letter == '$')
	{
		current = current->next;
		((t_token *)current->content)->lexem = VARIABLE;
		ft_lstpop(current->prev, free_token);
	}
}

void	lexer(t_cmd_tab *cmd_tab)
{
	scan(cmd_tab);
	apply_to_list(cmd_tab, cmd_tab->token_list, remove_space);
	apply_to_list(cmd_tab, cmd_tab->token_list, id_heredoc);
	apply_to_list(cmd_tab, cmd_tab->token_list, id_redirections);
	print_tokens(cmd_tab->token_list);
	ast_test(cmd_tab);
}

// TO DO
// escape character
// when to expand variables
// id variables

