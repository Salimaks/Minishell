/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 15:37:36 by mkling            #+#    #+#             */
/*   Updated: 2024/12/14 16:24:49 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* connect pipe for a fork */
void	connect_pipe(t_shell *shell, int *pipe_fd, int type)
{
	if (catch_error(shell))
		return ;
	if (type == WRITE)
	{
		close(pipe_fd[0]);
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);
	}
	if (type == READ)
	{
		close(pipe_fd[1]);
		dup2(pipe_fd[0], STDIN_FILENO);
		close(pipe_fd[0]);
	}
}

void	put_arg_in_array(t_cmd *cmd)
{
	int	argc;
	int	index;

	argc = ft_lstsize(cmd->arg_list);
	cmd->argv = ft_calloc(sizeof(char *), argc + 1);
	fork_exit_if(!cmd->argv, MALLOC_FAIL, cmd, "Failed to malloc argv");
	index = 0;
	while (index < argc)
	{
		cmd->argv[index] = cmd->arg_list->content;
		cmd->arg_list = cmd->arg_list->next;
		index++;
	}
}

/* If fork, get command path and execve the cmd or send error */
void	send_fork_exec_cmd(t_shell *shell, t_cmd *cmd)
{
	if (cmd->fork_pid != 0)
		return ;
	get_cmd_path(shell, cmd);
	put_arg_in_array(cmd);
	execve(cmd->cmd_path, cmd->argv, shell->env);
	fork_exit_if(1, CANT_EXECUTE_CMD, cmd, "Failed to execute command");
}

/* closes pipe and redirects to stdin */
void	close_pipe(t_shell *shell, int *pipe_fd)
{
	if (catch_error(shell))
		return ;
	close(pipe_fd[WRITE]);
	dup2(pipe_fd[READ], STDIN_FILENO);
	close(pipe_fd[READ]);
}

/* Wait on fork with its forkpid, Set exit code in cmd struct*/
void	wait_on_fork(t_shell *shell, t_cmd *cmd)
{
	if (!cmd->exit_code)
		waitpid(cmd->fork_pid, &cmd->exit_code, 0);
	apply_to_list(shell, cmd->infiles, destroy_heredoc);
}

int	execute_cmd(t_shell *shell, t_cmd *cmd)
{
	create_fork(shell, &cmd->fork_pid);
	redirect_fork(shell, cmd);
	send_fork_exec_cmd(shell, cmd);
	wait_on_fork(shell, cmd);
	return (cmd->exit_code);
}

int	execute_pipe(t_shell *shell, t_ast *ast)
{
	int	pipe_fd[2];
	int	fork_pid[2];
	int	last_exit_code;

	last_exit_code = 0;
	open_pipe(shell, pipe_fd);
	create_fork(shell, &fork_pid[0]);
	if (fork_pid[0] == 0)
	{
		connect_pipe(shell, pipe_fd, WRITE);
		exit(process_ast(shell, ast->left));
	}
	create_fork(shell, &fork_pid[1]);
	if (fork_pid[1] == 0)
	{
		connect_pipe(shell, pipe_fd, READ);
		exit(process_ast(shell, ast->left));
	}
	close_pipe(shell, pipe_fd);
	waitpid(fork_pid[0], NULL, 0);
	waitpid(fork_pid[1], &last_exit_code, 0);
	return (last_exit_code);
}

int	process_ast(t_shell *shell, t_ast *ast)
{
	if (ast->type == AST_CMD)
		return (execute_cmd(shell, ((t_cmd *)ast->content)));
	if (ast->type == AST_PIPE)
		return (execute_pipe(shell, ast));
	if (ast->type == AST_AND)
	{
		if (process_ast(shell, ast->left) == 0)
			return (process_ast(shell, ast->right));
		return (-1);
	}
	if (ast->type == AST_OR)
	{
		if (process_ast(shell, ast->left) != 0)
			return (process_ast(shell, ast->right));
		return (-1);
	}
	else
		return (EXIT_FAILURE);
}
