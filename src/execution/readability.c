/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readability.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 18:37:37 by mkling            #+#    #+#             */
/*   Updated: 2024/12/26 13:52:27 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	create_fork(t_shell *shell, int	*fork_pid)
{
	if (catch_error(shell))
		return ;
	*fork_pid = fork();
	set_error_if(*fork_pid == -1, FORK_ERROR, shell,
		"Error while trying to fork");
}

void	close_pipe(t_shell *shell, int *pipe_fd)
{
	if (catch_error(shell))
		return ;
	close(pipe_fd[READ]);
	close(pipe_fd[WRITE]);
}

