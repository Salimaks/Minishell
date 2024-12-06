/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akling <akling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 18:04:03 by alex              #+#    #+#             */
/*   Updated: 2024/12/06 10:51:10 by akling           ###   ########.fr       */
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

void	insert_token_at_end(t_token *token, t_cmd_tab *cmd_tab)
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

	new_token = (t_token *)ft_calloc(sizeof(t_token), 1);
	if (!new_token)
		return (set_error(MALLOC_FAIL, cmd_tab,
				"Failed to allocate token"));
	new_token->content = ft_calloc(sizeof(char), 2);
	if (!new_token->content)
		return (set_error(MALLOC_FAIL, cmd_tab,
				"Failed to allocate token content"));
	new_token->lexem = lexem;
	new_token->letter = letter;
	new_token->content[0] = letter;
	new_token->content[1] = '\0';
	insert_token_at_end(new_token, cmd_tab);
}

void	pop_token(t_token *token)
{
	t_token	*tmp;

	tmp = token;
	if (!token)
		return ;
	if (token->prev == NULL)
		token = token->next;
	else
		token->prev->next = token->next;
	if (token->next != NULL)
		token->next->prev = token->prev;
	free_token(tmp);
}

void	apply_to_token_list(t_cmd_tab *cmd_tab, t_token *token,
			void function(t_cmd_tab*, t_token*))
{
	t_token	*current;

	current = token;
	while (current != NULL)
	{
		function(cmd_tab, current);
		current = current->next;
	}
}
