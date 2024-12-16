/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readability.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 18:37:37 by mkling            #+#    #+#             */
/*   Updated: 2024/12/16 15:49:17 by mkling           ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "minishell.h"

int	is_last_cmd(t_shell *shell, t_list *cmd_node)
{
	return (cmd_node->next == NULL);
}

int	is_first_cmd(t_shell *shell, t_list *cmd_node)
{
	return (cmd_node->prev == NULL);
}

void	create_fork(t_shell *shell, int	*fork_pid)
{
	if (catch_error(shell))
		return ;
	*fork_pid = fork();
	set_error_if(*fork_pid == -1, FORK_ERROR, shell, 
		"Error while trying to fork");
}

void	close_pipe(t_shell *shell, t_list *node)
{
	if (catch_error(shell) || !node->next)
		return ;
	close(((t_cmd *)node->content)->pipe_fd[READ]);
	close(((t_cmd *)node->content)->pipe_fd[WRITE]);
}

