/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 00:22:21 by alex              #+#    #+#             */
/*   Updated: 2024/11/30 00:30:15 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_token(t_token *token)
{
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
