/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 14:09:43 by mkling            #+#    #+#             */
/*   Updated: 2024/12/18 16:18:28 by mkling           ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "minishell.h"


int	main(int argc, char **argv, char *envp[])
{
	t_shell	*shell;
	char s[100];

	(void)argc;
	(void)argv;
	shell = create_minishell(envp);
	fprintf(stderr, "%s\n", getcwd(s, 100));
	cd(shell, NULL);
	fprintf(stderr, "%s\n", getcwd(s, 100));
	fprintf(stderr, "success");
	return (0);
}
