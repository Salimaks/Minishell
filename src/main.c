/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 14:09:43 by mkling            #+#    #+#             */
/*   Updated: 2024/12/18 18:37:25 by mkling           ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "minishell.h"


int	main(int argc, char **argv, char **envp)
{
	t_shell *shell;

	(void)argc,
	(void)argv,
	shell = create_minishell(envp);
	printf("hello?\n");
	pwd(STDOUT_FILENO);
	exit_shell(shell);
	printf("gooodbyeeeee?\n");
	return (0);
}
