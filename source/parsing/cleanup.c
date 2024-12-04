/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 00:22:21 by alex              #+#    #+#             */
/*   Updated: 2024/12/03 11:18:28 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_token(t_token *token)
{
	if (token == NULL)
		return ;
	if (token->content != NULL)
		free(token->content);
	free(token);
}

void	free_token_list(t_cmd_tab *cmd_tab)
{
	t_token	*current;
	t_token	*next;

	current = cmd_tab->token_list;
	while (current != NULL)
	{
		next = current->next;
		free_token(current);
		current = next;
	}
	cmd_tab->token_list = NULL;
}

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
