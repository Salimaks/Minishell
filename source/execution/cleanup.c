/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 14:37:02 by mkling            #+#    #+#             */
/*   Updated: 2024/11/29 13:29:15 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	free_cmd(t_cmd *cmd)
{
	free(cmd->argv);
	free(cmd->env);
	free(cmd->outfile);
	free(cmd->infile);
	free(cmd->cmd_path);
	free(cmd);
}

void	free_cmd_list(t_cmd_tab *cmd_tab)
{
	t_cmd	*current;
	t_cmd	*next;

	current = cmd_tab->cmd_list;
	while (current->next != NULL)
	{
		next = current->next;
		free_cmd(current);
		current = next;
	}
	free(cmd_tab->cmd_list);
}

void	free_cmd_tab(t_cmd_tab *cmd_tab)
{
	cmd_tab->index = 0;
	free(cmd_tab->cmd_list);
	free(cmd_tab);
}
