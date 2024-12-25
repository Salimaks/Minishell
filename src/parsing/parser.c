/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 13:06:39 by alex              #+#    #+#             */
/*   Updated: 2024/12/24 11:49:10 by alex             ###   ########.fr       */
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
		((t_token *)(*current)->content)->lexem = OUTFILE;
	}
	create_file(shell, cmd, ((t_token *)(*current)->content));
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
}

t_tree	*parse_command(t_shell *shell, t_list **node)
{
	t_cmd	*cmd;
	t_token	*token;

	if (!(*node))
		return (set_error(CANT_FIND_CMD, shell, "Missing command"), NULL);
	cmd = create_cmd();
	token = ((t_token *)(*node)->content);
	while (token->letter != PIPE && token->lexem != END)
	{
		if (token->letter == '<')
			parse_infiles(shell, cmd, node);
		else if (token->letter == '>')
			parse_outfiles(shell, cmd, node);
		else if (token->lexem == WORD || token->lexem == STRING)
			ft_lstadd_back(&cmd->arg_list,
				ft_lstnew(ft_strdup(token->content)));
		*node = (*node)->next;
		token = ((t_token *)(*node)->content);
	}
	return (create_branch(shell, AST_CMD, cmd));
}

t_tree	*parse_pipe(t_shell *shell, t_list **token)
{
	t_tree	*left;
	t_tree	*right;
	t_tree	*pipe_node;

	left = parse_command(shell, token);
	if (!left)
		return (NULL);
	if (!(*token) || ((t_token *)(*token)->content)->letter != PIPE)
		return (left);
	*token = (*token)->next;
	right = parse_pipe(shell, token);
	if (!right)
		return (free_tree(&left), NULL);
	pipe_node = create_branch(shell, AST_PIPE, NULL);
	pipe_node->left = left;
	pipe_node->right = right;
	return (pipe_node);
}

void	parser(t_shell *shell)
{
	shell->tree_root = parse_pipe(shell, &shell->token_list);
	ft_lstclear(&shell->token_list, free_token);
}
