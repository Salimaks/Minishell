/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skassimi <skassimi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 09:20:08 by skassimi          #+#    #+#             */
/*   Updated: 2025/01/09 14:18:51 by skassimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	extract_env_as_linked_list(t_shell *shell)
{
	int		index;

	index = 0;
	while (shell->env[index])
	{
		ft_lstadd_back(&shell->env_list,
			ft_lstnew(ft_strdup(shell->env[index])),ENV);
		index++;
	}
}

int	env(t_shell *shell, int fdout)
{
	while (shell->env_list != NULL)
	{
		ft_putstr_fd((char *)shell->env_list->content, fdout);
		write(fdout, "\n", 1);
		shell->env_list = shell->env_list->next;
	}
	return (0);
}
