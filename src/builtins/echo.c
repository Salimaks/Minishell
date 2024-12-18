/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 09:19:21 by skassimi          #+#    #+#             */
/*   Updated: 2024/12/18 18:22:42 by mkling           ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

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
		ft_putstr_fd(argv[i], fdout);
		i++;
	}
	if (need_newline)
		write(fdout, "\n", 1);
	return (0);
}


