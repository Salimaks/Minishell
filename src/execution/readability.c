/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readability.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 18:37:37 by mkling            #+#    #+#             */
/*   Updated: 2024/12/14 16:16:04 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_last_cmd(t_shell *shell)
{
	return (shell->index == shell->cmd_count - 1);
}

int	is_first_cmd(t_shell *shell)
{
	return (shell->index == 0);
}

void	create_fork(t_shell *shell, int	*fork_pid)
{
	if (catch_error(shell))
		return ;
	*fork_pid = fork();
	set_error_if(*fork_pid == -1, FORK_ERROR, shell,
		"Error while trying to fork");
}

/*opens a pipe*/
void	open_pipe(t_shell *shell, int *pipe_fd)
{
	set_error_if(pipe(pipe_fd) == -1, PIPE_ERROR,
		shell, "Error while creating pipe");
}

