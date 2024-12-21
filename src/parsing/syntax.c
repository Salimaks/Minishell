/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 16:42:40 by mkling            #+#    #+#             */
/*   Updated: 2024/12/20 11:46:28 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	syntax_error(t_token *token)
{
	print_error();
	ft_putstr_fd("syntax error near unexpected token '", STDERR_FILENO);
	if (token->lexem == END)
		ft_putstr_fd("newline", STDERR_FILENO);
	else
		ft_putstr_fd(token->content, STDERR_FILENO);
	ft_putstr_fd("'\n", STDERR_FILENO);
}

void	is_missing_delimiter(t_shell *shell, t_list *node)
{
	t_token	*delim;
	t_token	*next_delim;

	if (catch_error(shell))
		return ;
	delim = (t_token *)node->content;
	if (delim->lexem != DELIMITER)
		return ;
	next_delim = (t_token *)node->next->content;
	while (next_delim->lexem != END)
	{
		if (((t_token *)node->content)->letter == delim->letter)
			return ;
		node = node->next;
		next_delim = (t_token *)node->content;
	}
	syntax_error(next_delim);
	shell->critical_er = SYNTAX_ERROR;
	return ;
}

void	is_missing_redirection(t_shell *shell, t_list *node)
{
	t_token	*next_token;
	t_token	*redirection;

	if (catch_error(shell))
		return ;
	redirection = (t_token *)node->content;
	if (redirection->letter != '<' && redirection->letter != '>')
		return ;
	next_token = (t_token *)node->next->content;
	if (next_token->lexem != WORD)
	{
		syntax_error(next_token);
		shell->critical_er = SYNTAX_ERROR;
		return ;
	}
	return ;
}

void	is_missing_cmd_before_pipe(t_shell *shell, t_list *node)
{
	t_token	*pipe_token;
	t_token	*prev_token;

	if (catch_error(shell))
		return ;
	pipe_token = (t_token *)node->content;
	if (pipe_token->letter != '|')
		return ;
	prev_token = (t_token *)node->prev->content;
	while (prev_token->lexem != START && prev_token->lexem != PIPE)
	{
		if (prev_token->lexem == WORD)
			return ;
		node = node->prev;
		prev_token = (t_token *)node->prev->content;
	}
	syntax_error(pipe_token);
	shell->critical_er = SYNTAX_ERROR;
	return ;
}

void	is_missing_cmd_after_pipe(t_shell *shell, t_list *node)
{
	t_token	*pipe_token;
	t_token	*next_token;

	if (catch_error(shell))
		return ;
	pipe_token = (t_token *)node->content;
	if (pipe_token->letter != '|')
		return ;
	next_token = (t_token *)node->next->content;
	while (next_token->lexem != END && next_token->lexem != PIPE)
	{
		if (next_token->lexem == WORD)
			return ;
		node = node->prev;
		next_token = (t_token *)node->prev->content;
	}
	syntax_error(pipe_token);
	shell->critical_er = SYNTAX_ERROR;
	return ;
}

int	check_syntax(t_shell *shell, t_list *node)
{
	apply_to_list(shell, node, is_missing_cmd_before_pipe);
	apply_to_list(shell, node, is_missing_cmd_after_pipe);
	apply_to_list(shell, node, is_missing_redirection);
	apply_to_list(shell, node, is_missing_delimiter);
	return (shell->critical_er);
}
