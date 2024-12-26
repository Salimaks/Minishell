/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 21:51:24 by mkling            #+#    #+#             */
/*   Updated: 2024/12/24 11:44:58 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_tree	*create_branch(t_shell *shell, int type, void *content)
{
	t_tree	*node;

	node = ft_calloc(1, sizeof(t_tree));
	if (!node)
		return (set_error(MALLOC_FAIL, shell, "Failed to malloc tree"), NULL);
	node->type = type;
	node->content = content;
	node->left = NULL;
	node->left = NULL;
	return (node);
}

void	put_arg_in_array(t_cmd *cmd)
{
	int	argc;
	int	index;

	argc = ft_lstsize(cmd->arg_list);
	cmd->argv = ft_calloc(sizeof(char *), argc + 1);
	if (!cmd->argv)
		return (set_cmd_error(MALLOC_FAIL, cmd, "Failed to malloc argv"));
	index = 0;
	while (index < argc)
	{
		cmd->argv[index] = cmd->arg_list->content;
		cmd->arg_list = cmd->arg_list->next;
		index++;
	}
}

int	pipe_exec_tree(t_shell *shell, t_tree *tree, int pipe_fd[2], int mode)
{
	int	exit_code;

	if (mode == WRITE)
	{
		close(pipe_fd[READ]);
		dup2(pipe_fd[WRITE], STDOUT_FILENO);
		close(pipe_fd[WRITE]);
	}
	if (mode == READ)
	{
		close(pipe_fd[WRITE]);
		dup2(pipe_fd[READ], STDIN_FILENO);
		close(pipe_fd[READ]);
	}
	exit_code = exec_tree(shell, tree, true);
	exit (exit_code);
}

int	exec_tree(t_shell *shell, t_tree *tree, bool piped)
{
	int	exit_code;

	if (!tree)
		return (set_error(SYNTAX_ERROR, shell, "Empty tree"), SYNTAX_ERROR);
	if (tree->type == AST_PIPE)
		return (exec_pipe(shell, tree));
	if (tree->type == AST_AND)
	{
		exit_code = exec_tree(shell, tree->left, NO_PIPE);
		if (exit_code == 0)
			return (exec_tree(shell, tree->right, NO_PIPE));
		return (exit_code);
	}
	if (tree->type == AST_OR)
	{
		exit_code = exec_tree(shell, tree->left, NO_PIPE);
		if (exit_code != 0)
			return (exit_code);
		return (exec_tree(shell, tree->right, NO_PIPE));
	}
	return (exec_single_cmd(shell, tree, piped));
}

