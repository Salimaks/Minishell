/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 09:19:30 by skassimi          #+#    #+#             */
/*   Updated: 2024/12/18 16:19:30 by mkling           ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "minishell.h"

char	*extract_home(t_shell *shell)
{
	char	*path;

	while (shell->env_list)
	{
		path = (char *)shell->env_list->content;
		if (ft_strncmp(path, "HOME=", 5) == 0)
			return (&path[5]);
		shell->env_list = shell->env_list->next;
	}
	return (NULL);
}


int	cd(t_shell *shell, char *path)
{
	int	exit_code;

	if (path == NULL)
		path = extract_home(shell);
	exit_code = chdir(path);
	if (exit_code != 0)
		perror("wtf");
	return (exit_code);
}


// cd sans argument renvoie vers le home
