/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 16:54:16 by mkling            #+#    #+#             */
/*   Updated: 2024/12/30 12:32:28 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// tokenize "" or '' as single empty string to ensure ""'$USER'"" and ''$USER''
// expand as they do in bash


/* Applies requirements for expansion,
NEEDS TO BE ADAPTED */
int	can_expand(t_list *node)
{
	if (((t_token *)node->prev->content)->letter == '\'')
		return (0);
	return (1);
}

int	has_valid_var(char *string)
{
	char	first_letter;

	if (!string)
		return (0);
	first_letter = *(ft_strchr(string, '$') + 1);
	if (first_letter == '\0' || ft_isdigit(first_letter))
		return (0);
	return (1);
}

void	expand_var(t_shell *shell, char **var_name)
{
	char	*var;

	if (ft_strcmp(*var_name, "$?"))
		var = ft_itoa(shell->last_exit_code);
	else
		var = ft_strdup((char *)find_env(shell->env_list, &(*var_name)[1])->content);
	free(*var_name);
	*var_name = var;
}

void	expand(t_shell *shell, t_list *node)
{
	t_token	*token;

	if (shell->critical_er || !node || !can_expand(node))
		return ;
	token = (t_token *)node->content;
	if (!has_valid_var((char *)token->content))
		return ;

}
