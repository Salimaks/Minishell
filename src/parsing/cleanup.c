/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 00:22:21 by alex              #+#    #+#             */
/*   Updated: 2024/12/11 21:15:23 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_token(void *to_be_del)
{
	t_token	*token;

	token = (t_token *)to_be_del;
	if (token == NULL)
		return ;
	if (token != NULL)
		free(((t_token *)token)->content);
	free(token);
}

void	free_file(void *to_be_del)
{
	t_file	*file;

	file = (t_file *)to_be_del;
	if (file == NULL)
		return ;
	if (file->path != NULL)
		free(file->path);
	if (file->delimiter != NULL)
		free(file->delimiter);
	free(file);
}

void	free_cmd(void *to_be_del)
{
	t_cmd	*cmd;

	cmd = (t_cmd *)to_be_del;
	if (cmd->argv != NULL)
		ft_free_tab(cmd->argv);
	if (cmd->cmd_path != NULL)
		free(cmd->cmd_path);
	ft_lstclear(&cmd->outfiles, free_file);
	ft_lstclear(&cmd->infiles, free_file);
	free(cmd);
}

void	free_cmd_tab(t_cmd_tab *cmd_tab)
{
	cmd_tab->index = 0;
	if (cmd_tab->cmd_list)
		ft_lstclear(&cmd_tab->cmd_list, free_cmd);
	if (cmd_tab->paths)
		ft_free_tab(cmd_tab->paths);
	if (cmd_tab->token_list)
		ft_lstclear(&cmd_tab->token_list, free_token);
	free(cmd_tab);
}
