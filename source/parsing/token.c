/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 18:04:03 by alex              #+#    #+#             */
/*   Updated: 2024/12/02 12:13:07 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*create_token(void)
{
	t_token	*token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->prev = NULL;
	token->next = NULL;
	token->content = NULL;
	token->lexem = NONE;
	return (token);
}

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

void	add_token(t_cmd_tab *cmd_tab, int lexem, char letter)
{
	t_token	*new_token;

	new_token = create_token();
	if (!new_token)
	{
		set_error(MALLOC_FAIL, cmd_tab,
			"Failed to allocate token");
		return ;
	}
	new_token->lexem = lexem;
	new_token->letter = letter;
	new_token->content = malloc(sizeof(char) * 2);
	if (!new_token->content)
	{
		set_error(MALLOC_FAIL, cmd_tab,
			"Failed to allocate token content");
		return ;
	}
	new_token->content[0] = letter;
	new_token->content[1] = '\0';
	append_token(new_token, cmd_tab);
}

void	pop_token(t_token *token)
{
	if (!token)
		return ;
	token->prev->next = token->next;
	token->next->prev = token->prev;
	free_token(token);
}
