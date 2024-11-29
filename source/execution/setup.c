/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 12:37:12 by alex              #+#    #+#             */
/*   Updated: 2024/11/29 17:29:07 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*get_last_cmd(t_cmd_tab *cmd_tab)
{
	t_cmd	*current;

	if (cmd_tab == NULL)
		return (NULL);
	current = cmd_tab->cmd_list;
	while (current->next != NULL)
		current = current->next;
	return (current);
}

void	append_cmd(t_cmd *cmd, t_cmd_tab *cmd_tab)
{
	t_cmd	*last_cmd;

	if (cmd_tab->cmd_list == NULL)
	{
		cmd_tab->cmd_list = cmd;
		cmd->cmd_index = 0;
	}
	else
	{
		last_cmd = get_last_cmd(cmd_tab);
		last_cmd->next = cmd;
		cmd->prev = last_cmd;
		cmd->cmd_index = last_cmd->cmd_index + 1;
	}
}

t_cmd	*create_cmd(void)
{
	t_cmd	*cmd;

	cmd = (t_cmd *)malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->prev = NULL;
	cmd->next = NULL;
	cmd->exit_code = 0;
	cmd->fork_pid = -1;
	return (cmd);
}

void	load_cmd(t_cmd_tab *cmd_tab, char **cmd_argv,
			char *outfile, char *infile)
{
	t_cmd	*new_cmd;

	new_cmd = create_cmd();
	if (!new_cmd)
	{
		set_error_if(!new_cmd, MALLOC_FAIL, cmd_tab,
			"Failed to allocate command structure");
		return ;
	}
	append_cmd(new_cmd, cmd_tab);
	new_cmd->argv = cmd_argv;
	new_cmd->outfile = outfile;
	new_cmd->infile = infile;
	cmd_tab->cmd_count = 1;
}

t_cmd_tab	*create_cmd_tab(char **env)
{
	t_cmd_tab	*cmd_tab;

	cmd_tab = ft_calloc(sizeof(t_cmd_tab), 1);
	if (!cmd_tab)
		return (NULL);
	cmd_tab->index = 0;
	cmd_tab->env = env;
	extract_paths(cmd_tab);
	return (cmd_tab);
}
