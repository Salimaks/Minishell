/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skassimi <skassimi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 09:19:50 by skassimi          #+#    #+#             */
/*   Updated: 2025/01/09 14:19:28 by skassimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	//alphabetic order ?
	while (argv[i])
	{
		unset(shell, &argv[i]);
		current_env = ft_lstnew(ft_strdup(argv[i]));
		if (!current_env || !current_env->content)
			return (-1);
		ft_lstadd_back(&shell->env_list, current_env, ENV);
		i++;
	}
	return (0);
}
