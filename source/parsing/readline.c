/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 13:51:38 by mkling            #+#    #+#             */
/*   Updated: 2024/11/29 17:32:27 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_readline(t_cmd_tab *cmd_tab)
{
	char	*input;

	while (1)
	{
		input = readline("Minishell> ");
		if (!input)
			break ;
		add_history(input);
		cmd_tab->cmd_line = input;
		printf("cmd line is %s\n", cmd_tab->cmd_line);
		free(input);
	}
}
