/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 15:37:36 by mkling            #+#    #+#             */
/*   Updated: 2024/12/16 15:03:04 by mkling           ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "minishell.h"

/* if not the last cmd, open pipe */
void	open_pipe(t_shell *shell, t_list *node)
{
	if (catch_error(shell) || !node->next)
		return ;
	fprintf(stderr, "opening pipe\n");
	set_error_if(pipe(((t_cmd *)node->content)->pipe_fd) == -1, PIPE_ERROR, shell,
		"Failed to pipe");
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

/* If fork, check command path, sends fork to execve, sets error if fail */
void	send_fork_exec_cmd(t_shell *shell, t_cmd *cmd)
{
	if (cmd->fork_pid != 0 || catch_error(shell))
		return ;
	fprintf(stderr, "executing %s\n", (char*)cmd->arg_list->content);
	put_arg_in_array(cmd);
	get_cmd_path(shell, cmd);
	fprintf(stderr, "executing %s\n", cmd->cmd_path);
	execve(cmd->cmd_path, cmd->argv, shell->env);
	fork_exit_if(1, CANT_EXECUTE_CMD, cmd, "Failed to execute command");
}

/* Wait on fork with forkpid, sets exit code */
void	wait_on_fork(t_shell *shell, t_list *cmd_list)
{
	t_cmd	*cmd;

	cmd = (t_cmd *)cmd_list->content;
	waitpid(cmd->fork_pid, &cmd->exit_code, 0);
	apply_to_list(shell, cmd->infiles, destroy_heredoc);
}

int	execute_all_cmd(t_shell *shell)
{
	t_cmd	*cmd;
	t_list	*node;

	if (catch_error(shell))
		return (catch_error(shell));
	node = shell->cmd_list;
	apply_to_list(shell, shell->cmd_list, open_pipe);
	while (node)
	{
		cmd = (t_cmd *)node->content;
		create_fork(shell, &cmd->fork_pid);
		redirect_fork(shell, node);
		send_fork_exec_cmd(shell, cmd);
		node = node->next;
	}
	apply_to_list(shell, shell->cmd_list, close_pipe);
	apply_to_list(shell, shell->cmd_list, wait_on_fork);
	return (get_last_cmd_exit_code(shell));
}
