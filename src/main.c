/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skassimi <skassimi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 09:47:55 by skassimi          #+#    #+#             */
/*   Updated: 2024/12/03 08:32:31 by skassimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

int exit_error()
{
}

int main(int agc, char **argv)
{
	t_command_table command_table;
	char *input;
	if(agc >1)
	{
		while(1)
		{
			command_table.first->infile = readline("minishell>");
			input = command_table.first->infile;

			if(!input)
				exit_error();/*a coder*/
			if(*input)
				add_history(input);
			free(input);
		}
	}
	clear_history();
	return(0);
}