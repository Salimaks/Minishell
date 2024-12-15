/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readability.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 18:37:37 by mkling            #+#    #+#             */
/*   Updated: 2024/12/15 18:52:41 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_last_cmd(t_shell *shell, t_list *node)
{
	return (((t_cmd *)node->content)->cmd_index == (size_t)ft_lstsize(shell->cmd_list) - 1);
}

int	is_first_cmd(t_shell *shell, t_list *node)
{
	return (node == shell->cmd_list);
}

void	create_fork(t_shell *shell, int	*fork_pid)
{
	if (catch_error(shell))
		return ;
	*fork_pid = fork();
	set_error_if(*fork_pid == -1, FORK_ERROR, shell, "Error while trying to fork");
}

/* close opened pipes*/
void	close_pipe(t_shell *shell, t_list *node)
{
	if (catch_error(shell) || !node->next)
		return ;
	close(((t_cmd *)node->content)->pipe_fd[READ]);
	close(((t_cmd *)node->content)->pipe_fd[WRITE]);
}

