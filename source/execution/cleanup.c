/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 14:37:02 by mkling            #+#    #+#             */
/*   Updated: 2024/12/02 18:37:48 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_cmd(t_cmd *cmd)
{
	if (cmd->argv != NULL)
	{
		ft_free_tab(cmd->argv);
		cmd->argv = NULL;
	}
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
	t_cmd	*prev;

	current = get_last_cmd(cmd_tab);
	while (current != NULL)
	{
		prev = current->prev;
		free_cmd(current);
		current = prev;
	}
	cmd_tab->cmd_list = NULL;
	cmd_tab->cmd_count = 0;
}

void	free_cmd_tab(t_cmd_tab *cmd_tab)
{
	cmd_tab->index = 0;
	if (cmd_tab->cmd_list)
		free_cmd_list(cmd_tab);
	if (cmd_tab->paths)
		ft_free_tab(cmd_tab->paths);
	free(cmd_tab);
}

void	set_error(int err_code, t_cmd_tab *cmd_tab, char *err_message)
{
	perror(err_message);
	cmd_tab->critical_er = err_code;
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
