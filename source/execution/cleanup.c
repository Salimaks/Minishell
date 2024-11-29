/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 14:37:02 by mkling            #+#    #+#             */
/*   Updated: 2024/11/29 16:36:17 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_cmd(t_cmd *cmd)
{
	if (cmd->argv != NULL)
		free(cmd->argv);
	if (cmd->outfile != NULL)
		free(cmd->outfile);
	if (cmd->outfile != NULL)
		free(cmd->infile);
	if (cmd->cmd_path != NULL)
		free(cmd->cmd_path);
	free(cmd);
}

void	free_cmd_list(t_cmd_tab *cmd_tab)
{
	t_cmd	*current;
	t_cmd	*next;

	current = cmd_tab->cmd_list;
	while (current != NULL)
	{
		next = current->next;
		free_cmd(current);
		current = next;
	}
}

void	free_cmd_tab(t_cmd_tab *cmd_tab)
{
	cmd_tab->index = 0;
	free_cmd_list(cmd_tab);
	if (cmd_tab->paths)
		ft_free_tab(cmd_tab->paths);
	free(cmd_tab);
}

void	set_error_if(int condition, int err_code, t_cmd_tab *cmd_tab,
		char *err_message)
{
	if (condition)
	{
		perror(err_message);
		cmd_tab->critical_er = err_code;
	}
}

int	catch_error(t_cmd_tab *cmd_tab)
{
	return (cmd_tab->critical_er);
}
