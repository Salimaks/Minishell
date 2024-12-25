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

void	reset_std(t_shell *shell, bool piped)
{
	if (piped)
		return ;
	dup2(shell->std_in, 0);
	dup2(shell->std_out, 1);
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

int	exec_with_fork(t_shell *shell, t_cmd *cmd)
{
	if (cmd->exit_code)
		return (cmd->exit_code);
	fprintf(stderr, "trying to exec %s\n", (char *)cmd->arg_list->content);
	cmd->fork_pid = fork();
	if (cmd->fork_pid == 0)
	{
		redirect_for_cmd(shell, cmd);
		get_cmd_path(shell, cmd);
		fprintf(stdout, "got path %s\n", cmd->cmd_path);
		put_arg_in_array(cmd);
		fprintf(stdout, "got argv %s\n", cmd->argv[0]);
		execve(cmd->cmd_path, cmd->argv, shell->env);
		set_cmd_error(CANT_EXECUTE_CMD, cmd, "Could not execute");
		exit(CANT_EXECUTE_CMD);
	}
	fprintf(stderr, "waiting on %s\n", (char *)cmd->arg_list->content);
	waitpid(cmd->fork_pid, &cmd->exit_code, 0);
	fprintf(stderr, "executed %s\n", (char *)cmd->arg_list->content);
	return (cmd->exit_code);
}

int	exec_single_cmd(t_shell *shell, t_tree *tree, int pipe_mode)
{
	t_cmd	*cmd;

	cmd = (t_cmd *)tree->content;
	if (!cmd->arg_list || cmd->exit_code)
	{
		redirect_for_cmd(shell, cmd);
		reset_std(shell, pipe_mode);
	}
	else if (is_builtin(cmd))
	{
		put_arg_in_array(cmd);
		redirect_for_cmd(shell, cmd);
		exec_builtin(shell, cmd);
		reset_std(shell, pipe_mode);
		return (cmd->exit_code);
	}
	else
		exec_with_fork(shell, cmd);
	return (cmd->exit_code);
}

int	exec_tree_piped(t_shell *shell, t_tree *tree, int pipe_fd[2], int pipe_mode)
{
	if (pipe_mode == WRITE)
	{
		close(pipe_fd[READ]);
		dup2(pipe_fd[WRITE], STDOUT_FILENO);
		close(pipe_fd[WRITE]);
	}
	if (pipe_mode == READ)
	{
		close(pipe_fd[WRITE]);
		dup2(pipe_fd[READ], STDIN_FILENO);
		close(pipe_fd[READ]);
	}
	return(exec_tree(shell, tree, true));
}

int	exec_pipe(t_shell *shell, t_tree *tree)
{
	int	pipe_fd[2];
	int	fork_pid1;
	int	fork_pid2;
	int	exit_code;

	if (pipe(pipe_fd) != 0)
		return (set_error(PIPE_ERROR, shell, "Failed to pipe"), PIPE_ERROR);
	fork_pid1 = fork();
	if (fork_pid1 < 0)
		return (set_error(FORK_ERROR, shell, "Failed to fork"), FORK_ERROR);
	if (fork_pid1 == 0)
		exec_tree_piped(shell, tree->left, pipe_fd, WRITE);
	else
	{
		fork_pid2 = fork();
		if (fork_pid2 < 0)
			return (set_error(FORK_ERROR, shell, "Failed to fork"), FORK_ERROR);
		if (fork_pid2 == 0)
			exec_tree_piped(shell, tree->right, pipe_fd, READ);
		else
		{
			close(pipe_fd[READ]);
			close(pipe_fd[WRITE]);
			waitpid(fork_pid1, &exit_code, 0);
			waitpid(fork_pid2, &exit_code, 0);
			return (exit_code);
		}
	}
	return (exit_code);
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

