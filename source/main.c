/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 14:09:43 by mkling            #+#    #+#             */
/*   Updated: 2024/11/30 00:10:29 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


int	main(int argc, char **env)
{
	t_cmd_tab	*cmd_tab;

	if (argc != 1)
		return (1);
	cmd_tab = create_cmd_tab(env);
	init_readline(cmd_tab);
	free_cmd_tab(cmd_tab);
	return (0);
}
