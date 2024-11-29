/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 14:09:43 by mkling            #+#    #+#             */
/*   Updated: 2024/11/29 13:32:46 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	main(int argc, char *argv[], char **env)
{
	t_cmd_tab *cmd_tab;

	if (argc > 1)
	{
		cmd_tab = create_cmd_tab(argv, env);
		printf("cmd tab index = %d\n", cmd_tab->index);
	}
	if (argc > 2)
		printf("ENV is %s of %s", env[0], argv[0]);
	return (0);
}
