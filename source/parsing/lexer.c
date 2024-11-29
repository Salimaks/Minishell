/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 17:34:47 by alex              #+#    #+#             */
/*   Updated: 2024/11/29 18:06:43 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*get_last_token(t_cmd_tab *cmd_tab)
{
	t_token	*current;

	if (cmd_tab == NULL)
		return (NULL);
	current = cmd_tab->token_list;
	while (current->next != NULL)
		current = current->next;
	return (current);
}

void	append_token(t_token *token, t_cmd_tab *cmd_tab)
{
	t_token	*last_token;

	if (cmd_tab->token_list == NULL)
		cmd_tab->token_list = token;
	else
	{
		last_token = get_last_token(cmd_tab);
		last_token->next = token;
		token->prev = last_token;
	}
}

t_token	*create_token(void)
{
	t_token	*token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->prev = NULL;
	token->next = NULL;
	token->type = NONE;
	token->subtype = NONE;
	return (token);
}

void	add_token(t_cmd_tab *cmd_tab, int type)
{
	t_token	*new_token;

	new_token = create_token();
	if (!new_token)
	{
		set_error_if(!new_token, MALLOC_FAIL, cmd_tab,
			"Failed to allocate token");
		return ;
	}
	new_token->type = type;
	append_cmd(new_token, cmd_tab);
}


void	tokenize(t_cmd_tab *cmd_tab)
{
	cmd_tab->index = 0;
	while (cmd_tab->cmd_line[cmd_tab->index])
	{
		id_separator(cmd_tab);
	}
}
