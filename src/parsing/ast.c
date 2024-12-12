/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 21:51:24 by mkling            #+#    #+#             */
/*   Updated: 2024/12/13 00:26:57 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ast	*create_ast_node(t_cmd_tab *cmd_tab, int type, void *content)
{
	t_ast	*node;

	node = ft_calloc(1, sizeof(t_ast));
	if (!node)
		return (set_error(MALLOC_FAIL, cmd_tab, "Failed to malloc ast"), NULL);
	node->type = type;
	node->content = content;
	node->left = NULL;
	node->left = NULL;
	return (node);
}

void	free_ast_node(t_ast *node)
{
	if (!node)
		return ;
	if (node->type == CMD)
		free_cmd(node->content);
	free(node);
}

void	parse_redirection(t_cmd_tab *cmd_tab, t_cmd *cmd, t_list **current)
{
	if (catch_error(cmd_tab)
		|| ((t_token *)(*current)->content)->lexem != OPERATOR)
		return ;
	else if (((t_token *)(*current)->content)->letter == '<')
	{
		(*current) = (*current)->next;
		((t_token *)(*current)->content)->lexem = INFILE;
		(*current) = (*current)->next;
	}
	else if (((t_token *)(*current)->content)->letter == '>')
	{
		(*current) = (*current)->next;
		((t_token *)(*current)->content)->lexem = OUTFILE;
		(*current) = (*current)->next;
	}
}


t_ast	*parse_cmd(t_cmd_tab *cmd_tab, t_list **token)
{
	t_ast	*cmd_node;
	t_cmd	*cmd;

	if (!(*token))
		return (set_error(CANT_FIND_CMD, cmd_tab, "Missing command"), NULL);
	cmd = ft_calloc(1, sizeof(t_cmd));
	if (!cmd)
		return (set_error(MALLOC_FAIL, cmd_tab, "Failed to malloc cmd"), NULL);
	cmd->fork_pid = -1;
	while (*token && ((t_token *)(*token)->content)->lexem != PIPE)
	{
		if (((t_token *)(*token)->content)->lexem != WORD)
			parse_redirection(cmd_tab,cmd, token);
		else
			ft_lstadd_back(&cmd->arg_list,
				ft_lstnew(ft_strdup(((t_token *)(*token)->content)->content)));
		(*token) = (*token)->next;
	}
	cmd_node = create_ast_node(cmd_tab, AST_CMD, cmd);
	return (cmd_node);
}

t_ast	*parse_pipe(t_cmd_tab *cmd_tab, t_list **token)
{
	t_ast	*left;
	t_ast	*right;
	t_ast	*pipe_node;

	left = parse_cmd(cmd_tab, token);
	if (!left)
		return (NULL);
	if (!(*token) || ((t_token *)(*token)->content)->lexem != PIPE)
		return (left);
	token = &(*token)->next;
	right = parse_pipe(cmd_tab, token);
	if (!right)
		return (free_ast_node(left), NULL);
	pipe_node = create_ast_node(cmd_tab, AST_PIPE, NULL);
	pipe_node->left = left;
	pipe_node->right = right;
	return (pipe_node);
}

void	process_ast(t_cmd_tab *cmd_tab, t_ast *ast_root)
{
	if (ast_root->type == AST_CMD)
		printf("execute cmd %s\n",
			((char *)((t_cmd*)ast_root->content)->arg_list->content));
	if (ast_root->type == AST_PIPE)
	{
		printf("execute pipe \n");
		process_ast(cmd_tab, ast_root->left);
		process_ast(cmd_tab, ast_root->right);
	}
}

void	ast_test(t_cmd_tab *cmd_tab)
{
	t_ast	*ast_root;

	ast_root = parse_pipe(cmd_tab, &cmd_tab->token_list);
	process_ast(cmd_tab, ast_root);
}
