/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 15:37:36 by mkling            #+#    #+#             */
/*   Updated: 2024/12/26 13:58:05 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_with_fork(t_shell *shell, t_cmd *cmd)
{
	if (cmd->exit_code)
		return (cmd->exit_code);
	cmd->fork_pid = fork();
	if (cmd->fork_pid == 0)
	{
		redirect_for_cmd(shell, cmd);
		get_cmd_path(shell, cmd);
		if (!cmd->cmd_path)
			exit(cmd->exit_code);
		put_arg_in_array(cmd);
		execve(cmd->cmd_path, cmd->argv, shell->env);
		exit(CANT_EXECUTE_CMD);
	}
	waitpid(cmd->fork_pid, &cmd->exit_code, 0);
	return (cmd->exit_code);
}

int	exec_single_cmd(t_shell *shell, t_tree *tree, bool piped)
{
	t_cmd	*cmd;

	cmd = (t_cmd *)tree->content;
	if (!cmd->arg_list || cmd->exit_code)
	{
		redirect_for_cmd(shell, cmd);
		reset_std(shell, piped);
	}
	else if (is_builtin(cmd))
	{
		put_arg_in_array(cmd);
		redirect_for_cmd(shell, cmd);
		exec_builtin(shell, cmd);
		reset_std(shell, piped);
		return (cmd->exit_code);
	}
	else
		exec_with_fork(shell, cmd);
	return (cmd->exit_code);
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
		pipe_exec_tree(shell, tree->left, pipe_fd, WRITE);
	else
	{
		fork_pid2 = fork();
		if (fork_pid2 < 0)
			return (set_error(FORK_ERROR, shell, "Failed to fork"), FORK_ERROR);
		if (fork_pid2 == 0)
			pipe_exec_tree(shell, tree->right, pipe_fd, READ);
		else
		{
			close_pipe(shell, pipe_fd);
			waitpid(fork_pid1, &exit_code, 0);
			waitpid(fork_pid2, &exit_code, 0);
			return (exit_code);
		}
	}
	return (PIPE_ERROR);
}

// /* If fork, check command path, sends fork to execve, sets error if fail */
// void	send_fork_exec_cmd(t_shell *shell, t_list *node)
// {
// 	t_cmd	*cmd;

// 	cmd = (t_cmd *)node->content;
// 	create_fork(shell, &cmd->fork_pid);
// 	if (catch_error(shell) || cmd->fork_pid != 0)
// 		return ;
// 	redirect_fork(shell, node);
// 	put_arg_in_array(cmd);
// 	get_cmd_path(shell, cmd);
// 	execve(cmd->cmd_path, cmd->argv, shell->env);
// 	set_cmd_error(CANT_EXECUTE_CMD, cmd, "Failed to execute command");
// 	exit(CANT_EXECUTE_CMD);
// }

// /* Wait on fork with forkpid, sets exit code */
// void	wait_on_fork(t_shell *shell, t_list *cmd_list)
// {
// 	t_cmd	*cmd;

// 	cmd = (t_cmd *)cmd_list->content;
// 	waitpid(cmd->fork_pid, &cmd->exit_code, 0);
// 	apply_to_list(shell, cmd->infiles, destroy_heredoc);
// }

// // TO - DO : Open only two pipes alternatively
// int	execute_all_cmd(t_shell *shell)
// {
// 	t_list	*node;

// 	if (catch_error(shell))
// 		return (catch_error(shell));
// 	node = shell->cmd_list;
// 	apply_to_list(shell, shell->cmd_list, open_pipe);
// 	while (node)
// 	{
// 		// if (is_builtin(cmd))
// 		// 	exec_builtin(shell, cmd);
// 		// else
// 		send_fork_exec_cmd(shell, node);
// 		node = node->next;
// 	}
// 	apply_to_list(shell, shell->cmd_list, close_pipe);
// 	apply_to_list(shell, shell->cmd_list, wait_on_fork);
// 	return (((t_cmd *)ft_lstlast(shell->cmd_list)->content)->exit_code);
// }
