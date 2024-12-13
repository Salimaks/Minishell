/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readability.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 18:37:37 by mkling            #+#    #+#             */
/*   Updated: 2024/12/13 16:16:56 by mkling           ###   ########.fr       */
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

t_list	*get_current_cmd_node(t_shell *shell)
{
	t_list	*node;

	node = shell->cmd_list;
	while (((t_cmd *)node->content)->cmd_index != shell->index)
		node = node->next;
	return (node);
}

t_cmd	*get_current_cmd(t_shell *shell)
{
	t_list	*node;

	node = shell->cmd_list;
	while (((t_cmd *)node->content)->cmd_index != shell->index)
		node = node->next;
	return (((t_cmd *)node->content));
}

void	create_fork(t_shell *shell, t_cmd *cmd)
{
	if (catch_error(shell))
		return ;
	cmd->fork_pid = fork();
	if (cmd->fork_pid == -1)
	{
		perror("Error while forking");
		cmd->exit_code = FORK_ERROR;
	}
}
