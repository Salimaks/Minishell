/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 16:42:40 by mkling            #+#    #+#             */
/*   Updated: 2024/12/16 18:12:55 by mkling           ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "minishell.h"

void	print_error(void)
{
	ft_putstr_fd(SHELL_NAME, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
}

void	syntax_error(char *token_content)
{
	print_error();
	ft_putstr_fd("syntax error near unexpected token '", STDERR_FILENO);
	ft_putstr_fd(token_content, STDERR_FILENO);
	ft_putstr_fd("'\n", STDERR_FILENO);
}

int	is_missing_redirection(t_shell *shell, t_list *node)
{
	t_token	*next_token;

	next_token = (t_token *)node->next->content;
	if (next_token->lexem != WORD && next_token->lexem != STRING)
	{
		if (next_token->lexem == END)
			syntax_error("newline");
		else
			syntax_error(next_token->content);
		shell->critical_er = SYNTAX_ERROR;
		return (SYNTAX_ERROR);
	}
	return (0);
}

int	is_missing_cmd_before_pipe(t_shell *shell, t_list *node)
{
	t_token	*pipe_token;
	t_token	*prev_token;

	pipe_token = (t_token *)node->content;
	prev_token = (t_token *)node->prev->content;
	while (prev_token->lexem != START && prev_token->lexem != PIPE)
	{
		if (prev_token->lexem == WORD || prev_token->lexem == STRING)
			return (0);
		node = node->next;
		prev_token = (t_token *)node->prev->content;
	}
	syntax_error(pipe_token->content);
	shell->critical_er = SYNTAX_ERROR;
	return (SYNTAX_ERROR);
}

int	check_syntax(t_shell *shell, t_list *node)
{
	t_token	*token;

	while (node->next)
	{
		token = (t_token *)node->content;
		if (token->letter == '|' && is_missing_cmd_before_pipe(shell, node))
			return (SYNTAX_ERROR);
		if ((token->letter == '<' || token->letter == '>')
			&& is_missing_redirection(shell, node))
			return (SYNTAX_ERROR);
		node = node->next;
	}
	return (0);
}
