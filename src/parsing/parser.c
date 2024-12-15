/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 13:06:39 by alex              #+#    #+#             */
/*   Updated: 2024/12/15 18:01:07 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	parse_outfiles(t_shell *shell, t_cmd *cmd, t_list **current)
{
	if (catch_error(shell))
		return ;
	if (((t_token *)(*current)->next->content)->letter == '>')
	{
		if (((t_token *)(*current)->next->next->content)->lexem != WORD)
			return (set_error(SYNTAX_ERROR, shell, "No append outfile"));
		*current = (*current)->next->next;
		((t_token *)(*current)->content)->lexem = APPEND;
	}
	else
	{
		if (((t_token *)(*current)->next->content)->lexem != WORD)
			return (set_error(SYNTAX_ERROR, shell, "No outfile"));
		*current = (*current)->next;
		((t_token *)(*current)->content)->lexem = INFILE;
	}
	create_file(shell, cmd, ((t_token *)(*current)->content));
	(*current) = (*current)->next;
}

void	parse_infiles(t_shell *shell, t_cmd *cmd, t_list **current)
{
	if (catch_error(shell))
		return ;
	if (((t_token *)(*current)->next->content)->letter == '<')
	{
		if (((t_token *)(*current)->next->next->content)->lexem != WORD)
			return (set_error(SYNTAX_ERROR, shell, "No delimiter"));
		*current = (*current)->next->next;
		((t_token *)(*current)->content)->lexem = HEREDOC;
	}
	else
	{
		if (((t_token *)(*current)->next->content)->lexem != WORD)
			return (set_error(SYNTAX_ERROR, shell, "No infile"));
		*current = (*current)->next;
		((t_token *)(*current)->content)->lexem = INFILE;
	}
	create_file(shell, cmd, ((t_token *)(*current)->content));
	(*current) = (*current)->next;
}

t_ast	*parse_command(t_shell *shell, t_list **token)
{
	t_cmd	*cmd;

	if (!(*token))
		return (set_error(CANT_FIND_CMD, shell, "Missing command"), NULL);
	cmd = ft_calloc(1, sizeof(t_cmd));
	if (!cmd)
		return (set_error(MALLOC_FAIL, shell, "Failed to malloc cmd"), NULL);
	cmd->fork_pid = -1;
	while (((t_token *)(*token)->content)->letter != PIPE
			&& ((t_token *)(*token)->content)->lexem != END)
	{
		if (((t_token *)(*token)->content)->lexem == START)
			*token = (*token)->next;
		if (((t_token *)(*token)->content)->letter == '<')
			parse_infiles(shell, cmd, token);
		else if (((t_token *)(*token)->content)->letter == '>')
			parse_outfiles(shell, cmd, token);
		else
			ft_lstadd_back(&cmd->arg_list,
				ft_lstnew(ft_strdup(((t_token *)(*token)->content)->content)));
		*token = (*token)->next;
	}
	return (create_ast_node(shell, AST_CMD, cmd));
}

t_ast	*parse_pipe(t_shell *shell, t_list **token)
{
	t_ast	*left;
	t_ast	*right;
	t_ast	*pipe_node;

	left = parse_command(shell, token);
	if (!left)
		return (NULL);
	if (!(*token) || ((t_token *)(*token)->content)->letter != PIPE)
		return (left);
	*token = (*token)->next;
	right = parse_pipe(shell, token);
	if (!right)
		return (free_ast(left), NULL);
	pipe_node = create_ast_node(shell, AST_PIPE, NULL);
	pipe_node->left = left;
	pipe_node->right = right;
	return (pipe_node);
}

void	parser(t_shell *shell)
{
	shell->ast_root = parse_pipe(shell, &shell->token_list);
	ft_lstclear(&shell->token_list, free_token);
}
