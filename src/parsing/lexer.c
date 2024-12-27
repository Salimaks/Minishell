/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 17:34:47 by alex              #+#    #+#             */
/*   Updated: 2024/12/27 14:32:39 by mkling           ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "minishell.h"

int	is_blank(t_list *node)
{
	if (!node || !node->content)
		return (0);
	return (((t_token *)node->content)->lexem == BLANK);
}

void	remove_space(t_shell *shell, t_list *current)
{
	if (catch_error(shell) || !current->next || !is_blank(current->next))
		return ;
	ft_lstpop(&shell->token_list, current->next, free_token);
}

void	group_words(t_shell *shell, t_list *node)
{
	if (!node->next || !node->next->content
		|| ((t_token *)node->next->content)->lexem == END)
		return ;
	while (((t_token *)node->content)->lexem == WORD
		&& ((t_token *)node->next->content)->lexem == WORD)
		merge_token(shell, node);
	while (((t_token *)node->content)->lexem == BLANK
		&& ((t_token *)node->next->content)->lexem == BLANK)
		merge_token(shell, node);
}

void	group_strings(t_shell *shell, t_list *node)
{
	t_token	*first_delim;

	first_delim = ((t_token *)node->content);
	if (first_delim->lexem != DELIMITER)
		return ;
	first_delim->content = ft_calloc(1, sizeof(char));
	if (!first_delim->content)
		return (set_error(MALLOC_FAIL, shell, "Failed to malloc string"));
	while (((t_token *)node->next->content)->lexem != END)
	{
		if (((t_token *)node->next->content)->letter == first_delim->letter)
		{
			ft_lstpop(&shell->token_list, node->next, free_token);
			break ;
		}
		merge_token(shell, node);
	}
	first_delim->lexem = STRING;
}

void	id_variables(t_shell * shell, t_list *current)
{
	if (catch_error(shell)
		|| ((t_token *)current->content)->lexem != OPERATOR)
		return ;
	if (((t_token *)current->content)->letter == '$')
	{
		merge_token(shell, current);
		((t_token *)current->content)->lexem = VARIABLE;
	}
}

void	lexer(t_shell *shell)
{
	scan(shell);
	apply_to_list(shell, shell->token_list, group_words);
	apply_to_list(shell, shell->token_list, group_strings);
	apply_to_list(shell, shell->token_list, remove_space);
	apply_to_list(shell, shell->token_list, id_variables);
	apply_to_list(shell, shell->token_list, expand_var);
	print_tokens(shell->token_list);
}

// TO DO
// escape character
// when to expand variables
// id variables

