/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 00:22:21 by alex              #+#    #+#             */
/*   Updated: 2024/12/15 18:00:48 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_token(void *to_be_del)
{
	t_token	*token;

	token = (t_token *)to_be_del;
	if (token == NULL)
		return ;
	if (token != NULL)
		free(((t_token *)token)->content);
	free(token);
}

void	free_file(void *to_be_del)
{
	t_file	*file;

	file = (t_file *)to_be_del;
	if (file == NULL)
		return ;
	if (file->path != NULL)
		free(file->path);
	if (file->delim != NULL)
		free(file->delim);
	free(file);
}

void	free_cmd(void *to_be_del)
{
	t_cmd	*cmd;

	cmd = (t_cmd *)to_be_del;
	if (cmd->argv != NULL)
		ft_free_tab(cmd->argv);
	if (cmd->cmd_path != NULL)
		free(cmd->cmd_path);
	ft_lstclear(&cmd->outfiles, free_file);
	ft_lstclear(&cmd->infiles, free_file);
	free(cmd);
}

void	free_ast(void *node)
{
	t_ast	*ast;

	if (!node)
		return ;
	ast = (t_ast *)node;
	if (ast->type == CMD)
		free_cmd(ast->content);
	else
	{
		free_ast(ast->left);
		free_ast(ast->right);
	}
	free(ast);
}

void	free_minishell(t_shell *shell)
{
	shell->index = 0;
	if (shell->cmd_list)
		ft_lstclear(&shell->cmd_list, free_cmd);
	if (shell->paths)
		ft_free_tab(shell->paths);
	if (shell->token_list)
		ft_lstclear(&shell->token_list, free_token);
	if (shell->env_list)
		ft_lstclear(&shell->env_list, free);
	// if (shell->ast_root)
	// 	ft_lstclear(&shell->ast_root, free_ast_node);
	free(shell);
}
