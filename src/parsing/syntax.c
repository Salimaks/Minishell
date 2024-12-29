/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 16:42:40 by mkling            #+#    #+#             */
/*   Updated: 2024/12/28 19:26:35 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Minishell's subject states open quotes are not to be implemented */
void	is_missing_delimiter(t_shell *shell, t_list *node)
{
	t_list	*current;
	char	delim_type;

	if (shell->critical_er || !token_is(DELIMITER, node))
		return ;
	delim_type = (char)((t_token *)node->content)->letter;
	current = node->next;
	while (!token_is(END, current) && !token_is(END, current->next))
	{
		if (((t_token *)current->content)->letter == delim_type)
			return ;
		current = current->next;
	}
	print_syntax_error((t_token *)current->content);
	shell->critical_er = SYNTAX_ERROR;
}

/* Bash syntax expect word anytime after redirection to be file path */
void	is_missing_redirection(t_shell *shell, t_list *node)
{
	t_token	*operator;
	t_list	*current;

	operator = (t_token *)node->content;
	if (shell->critical_er || !token_is(OPERATOR, node)
		|| (operator->letter != '<' && operator->letter != '>'))
		return ;
	current = node->next;
	while (!token_is(END, current))
	{
		if (token_is(WORD, current))
			return ;
		current = current->next;
	}
	print_syntax_error((t_token *)current->content);
	shell->critical_er = SYNTAX_ERROR;
	return ;
}

void	is_missing_cmd_before_pipe(t_shell *shell, t_list *node)
{
	t_list	*current;

	if (shell->critical_er || !token_is(OPERATOR, node)
		|| ((t_token *)node->content)->letter != '|')
		return ;
	current = node->prev;
	while (!token_is(START, current) && !token_is(PIPE, current))
	{
		if (token_is(WORD, current))
			return ;
		current = current->prev;
	}
	print_syntax_error(((t_token *)node->content));
	shell->critical_er = SYNTAX_ERROR;
}

void	is_missing_cmd_after_pipe(t_shell *shell, t_list *node)
{
	t_list	*current;

	if (shell->critical_er || !token_is(OPERATOR, node)
		|| ((t_token *)node->content)->letter != '|')
		return ;
	current = node->next;
	while (!token_is(END, current) && !token_is(PIPE, current))
	{
		if (token_is(WORD, current))
			return ;
		current = current->next;
	}
	print_syntax_error(((t_token *)node->content));
	shell->critical_er = SYNTAX_ERROR;
}

int	check_syntax(t_shell *shell, t_list *node)
{
	apply_to_list(shell, node, is_missing_cmd_before_pipe);
	apply_to_list(shell, node, is_missing_cmd_after_pipe);
	apply_to_list(shell, node, is_missing_redirection);
	apply_to_list(shell, node, is_missing_delimiter);
	return (shell->critical_er);
}
