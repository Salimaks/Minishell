/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 09:47:59 by skassimi          #+#    #+#             */
/*   Updated: 2024/12/13 15:55:39 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_sigint(int sig)
{
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	(void) sig;
}

void	handle_siguit(int sig)
{
	(void)sig;
}

void	signals(void)
{
	signal(SIGINT, &handle_sigint);
	signal(SIGQUIT, &handle_siguit);
	signal(SIGTSTP, SIG_IGN);
}

/*int main(void)
{
	char *infile;
	signals();
	while(1)
	{
		infile = readline("shell>");
		if(!infile)
		{
			write(1, "exit\n", 5);
			break;
		}
		if(*infile)
			add_history(infile);
		free(infile);
	}
	return(0);
}*/
