/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 09:19:21 by skassimi          #+#    #+#             */
/*   Updated: 2024/12/26 14:33:24 by alex             ###   ########.fr       */
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
		if (i > 1)
			ft_putstr_fd(" ", fdout);
		ft_putstr_fd(argv[i], fdout);
		i++;
	}
	if (need_newline)
		write(fdout, "\n", 1);
	return (0);
}


