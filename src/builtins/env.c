/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 09:20:08 by skassimi          #+#    #+#             */
/*   Updated: 2024/12/12 20:19:29 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	extract_env_as_linked_list(t_cmd_tab *cmd_tab)
{
	int		index;

	index = 0;
	while (cmd_tab->env[index])
	{
		ft_lstadd_back(&cmd_tab->env_list,
			ft_lstnew(ft_strdup(cmd_tab->env[index])));
		index++;
	}
}
