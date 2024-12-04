/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 14:09:43 by mkling            #+#    #+#             */
/*   Updated: 2024/12/03 10:12:05 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


int	main(int argc, char **argv, char *envp[])
{
	if (argc != 1)
		return (1);
	(void)argv;
	init_readline(envp);
	return (0);
}
