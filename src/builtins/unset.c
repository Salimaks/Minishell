/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 09:19:57 by skassimi          #+#    #+#             */
/*   Updated: 2024/12/21 20:54:39 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*find_env(t_list *env_list, char *env_name)
{
	if (env_list == NULL || env_name == NULL)
		return (NULL);
	while (env_list)
	{
		if (ft_strcmp(env_name, (char *)env_list->content) == 0)
			return (env_list);
		env_list = env_list->next;
	}
	return (NULL);
}

int	unset(t_shell *shell, char **argv)
{
	int		i;
	t_list	*to_be_unset;

	i = 1;
	while (argv[i])
	{
		to_be_unset = find_env(shell->env_list, argv[i]);
		ft_lstpop(&shell->env_list, to_be_unset, free);
		i++;
	}
	return (0);
}
