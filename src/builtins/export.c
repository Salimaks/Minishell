/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 09:19:50 by skassimi          #+#    #+#             */
/*   Updated: 2024/12/19 14:01:35 by mkling           ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "minishell.h"

int	export(t_shell *shell, char **argv, int fdout)
{
	int		i;
	t_list	*current_env;

	i = 1;
	if (fdout < 0)
		return (-1);
	if (argv[i] == NULL )
	{
		current_env = shell->env_list;
		while (current_env != NULL)
		{
			ft_putstr_fd("export ", fdout);
			ft_putstr_fd((char *)current_env->content, fdout);
			write(fdout, "\n", 1);
			current_env = current_env->next;
		}
		return (0);
	}
	//alphatic tri
	while (argv[i])
	{
		unset (shell, &argv[i]);
		ft_lstadd_back(&shell->env_list, ft_lstnew(argv[i]));
		i++;
	}
	return (0);
}
