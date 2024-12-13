/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 21:51:24 by mkling            #+#    #+#             */
/*   Updated: 2024/12/13 16:52:19 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ast	*create_ast_node(t_shell *shell, int type, void *content)
{
	t_ast	*node;

	node = ft_calloc(1, sizeof(t_ast));
	if (!node)
		return (set_error(MALLOC_FAIL, shell, "Failed to malloc ast"), NULL);
	node->type = type;
	node->content = content;
	node->left = NULL;
	node->left = NULL;
	return (node);
}

void	free_ast_node(void *node)
{
	t_ast	*ast;

	if (!node)
		return ;
	ast = (t_ast *)node;
	if (ast->type == CMD)
		free_cmd(ast->content);
	else
	{
		free_ast_node(ast->left);
		free_ast_node(ast->right);
		free(ast->content);
	}
	free(ast);
}

/* Wait on a forks with its forkpid,
Set its exit code */
void	wait_on_fork(t_shell *shell, t_cmd *cmd)
{
	if (!cmd->exit_code)
		waitpid(cmd->fork_pid, &cmd->exit_code, 0);
	shell->index++;
	apply_to_list(shell, cmd->infiles, destroy_heredoc);
}

int		execute_cmd(t_shell *shell, t_cmd *cmd, int pipe_fd)
{
	create_fork(shell, cmd);
	redirect_fork(shell, cmd);
	send_fork_exec_cmd(shell, cmd);
	wait_on_fork(shell, cmd);
	return (cmd->exit_code);
}

int	process_ast(t_shell *shell, t_ast *ast)
{
	if (ast->type == AST_CMD)
		return (execute_cmd(shell, ((t_cmd *)ast->content), NULL));
	if (ast->type == AST_PIPE)
	{
		open_pipe(shell, ast->content);
		process_ast(shell, ast->left);
		process_ast(shell, ast->right);
		close_pipe(shell, ast->content);
	}
}
