/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 16:54:16 by mkling            #+#    #+#             */
/*   Updated: 2024/12/27 14:48:59 by mkling           ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "minishell.h"

// tokenize "" or '' as single empty string to ensure ""'$USER'"" and ''$USER''
// expand as they do in bash


/* Applies requirements for expansion,
NEEDS TO BE ADAPTED */
int	must_expand(t_list *node)
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

char	*import_var(t_shell *shell, char *variable)
{
	if (ft_strcmp(variable, "$?"))
		return (ft_itoa(shell->last_exit_code));
	else
		return((char *)find_env(shell->env_list, &variable[1])->content);
}

void	expand_var(t_shell *shell, char *string)
{
	int		i;
	char	*expanded_var;
	char	*left;

	if (!has_valid_var(string))
		return ;
	i = 0;
	while (string[i] != '$')
		i++;
	


}
