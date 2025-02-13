/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skassimi <skassimi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 09:19:21 by skassimi          #+#    #+#             */
/*   Updated: 2025/01/27 10:58:10 by skassimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	echo(char **argv, int fdout)
{
	int		i;
	bool	need_newline;

	if (fdout < 0)
		return (-1);
	need_newline = true;
	i = 1;
	if (ft_strcmp(argv[i], "-n") == 0)
	{
		i++;
		need_newline = false;
	}
	while (argv[i] != NULL)
	{
		if ((need_newline && i > 1) || (!need_newline && i > 2))
			ft_putstr_fd(" ", fdout);
		ft_putstr_fd(argv[i], fdout);
		i++;
	}
	if (need_newline)
		write(fdout, "\n", 1);
	return (0);
}


