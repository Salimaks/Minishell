/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 14:37:02 by mkling            #+#    #+#             */
/*   Updated: 2024/11/28 14:40:48 by mkling           ###   ########.fr       */
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
	free(cmd->pipe_fd);
	free(cmd);
}

void	free_cmd_tab(t_cmd_tab *cmd_tab)
{
	cmd_tab->index = 0;
	while (cmd_tab->index < cmd_tab->cmd_count)
	{
		free_cmd(cmd_tab->cmd_array[cmd_tab->index]);
		cmd_tab->index++;
	}
	free(cmd_tab->cmd_array);
	free(cmd_tab);
}
