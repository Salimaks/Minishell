/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 09:19:57 by skassimi          #+#    #+#             */
/*   Updated: 2024/12/19 13:40:29 by mkling           ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

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
	int	i;

	i = 1;
	while (argv[i])
	{
		ft_lstpop(find_env(shell->env_list, argv[i]), free);
		i++;
	}
	return (0);
}
