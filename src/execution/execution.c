/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 15:37:36 by mkling            #+#    #+#             */
/*   Updated: 2024/12/25 19:00:40 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// /* if not the last cmd, open pipe */
// void	open_pipe(t_shell *shell, t_list *node)
// {
// 	if (catch_error(shell) || !node->next)
// 		return ;
// 	set_error_if(pipe(((t_cmd *)node->content)->pipe_fd) == -1, PIPE_ERROR,
// 		shell, "Failed to pipe");
// }

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
