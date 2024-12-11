/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 00:22:21 by alex              #+#    #+#             */
/*   Updated: 2024/12/11 19:38:27 by mkling           ###   ########.fr       */
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

void	free_file(t_files *file)
{
	if (file == NULL)
		return ;
	if (file->filepath != NULL)
		free(file->filepath);
	if (file->delimiter != NULL)
		free(file->delimiter);
	free(file);
}

void	free_cmd(t_cmd *cmd)
{
	if (cmd->argv != NULL)
		ft_free_tab(cmd->argv);
	if (cmd->cmd_path != NULL)
		free(cmd->cmd_path);
	ft_lstclear(cmd->redirect, free_file);
	free(cmd);
}

void	free_cmd_tab(t_cmd_tab *cmd_tab)
{
	cmd_tab->index = 0;
	if (cmd_tab->cmd_list)
		ft_lstclear(cmd_tab->cmd_list, free_cmd);
	if (cmd_tab->paths)
		ft_free_tab(cmd_tab->paths);
	if (cmd_tab->token_list)
		ft_lstclear(cmd_tab->token_list, free_token);
	free(cmd_tab);
}
