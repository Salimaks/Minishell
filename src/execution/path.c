/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 14:12:32 by alex              #+#    #+#             */
/*   Updated: 2024/12/14 16:13:28 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Sets the command's table env to NULL
Extract path from env if env exists
Splits it into an array of possible path
Loads it into the command table */
void	extract_paths(t_shell *shell)
{
	char	*path;
	int		index;

	path = NULL;
	shell->paths = NULL;
	index = 0;
	while (shell->env[index])
	{
		if (ft_strncmp(shell->env[index], "PATH=", 5) == 0)
		{
			path = ft_substr(shell->env[index], 5,
					ft_strlen(shell->env[index]));
			set_error_if(!path, MALLOC_FAIL, shell,
				"Failed to allocate for path");
			shell->paths = ft_split(path, ':');
			set_error_if(!shell->paths, MALLOC_FAIL, shell,
				"Failed to split path variable");
			free(path);
		}
		index++;
	}
}

int	is_a_directory(char *path)
{
	int	fd;

	fd = open(path, __O_DIRECTORY);
	if (fd < 0)
		return (0);
	close (fd);
	return (1);
}

void	check_cmd(t_cmd *cmd)
{
	fork_exit_if((cmd->cmd_path == NULL), CANT_FIND_CMD, cmd,
		"No command found");
	fork_exit_if(is_a_directory(cmd->cmd_path), CANT_EXECUTE_CMD, cmd,
		"Command is a directory");
	fork_exit_if(access(cmd->cmd_path, X_OK) != 0, CANT_EXECUTE_CMD, cmd,
		"Command cannot be executed");
}

/* */
void	find_accessible_path(t_shell *shell, t_cmd *cmd)
{
	char	*tested_path;
	size_t	i;

	i = 0;
	while (shell->paths[i])
	{
		tested_path = ft_strjoin(shell->paths[i++], cmd->cmd_path);
		fork_exit_if(!tested_path, MALLOC_FAIL, cmd,
			"Failed to allocate path");
		if (access(tested_path, F_OK | R_OK) == 0)
		{
			cmd->cmd_path = tested_path;
			return ;
		}
		free(tested_path);
	}
	fork_exit_if(1, CANT_FIND_CMD, cmd, "No command found");
}

/* Checks first absolute path for command
Then paths if any were extracted from env
Exits fork if no command is found */
void	get_cmd_path(t_shell *shell, t_cmd *cmd)
{
	cmd->cmd_path = ft_strjoin("/", (char *)cmd->arg_list->content);
	fork_exit_if((!cmd->cmd_path), MALLOC_FAIL, cmd,
		"Failed to allocate path");
	if (access(cmd->cmd_path, F_OK) == 0)
		return (check_cmd(cmd));
	fork_exit_if(shell->paths == NULL, CANT_FIND_CMD, cmd,
		"No PATH variable found");
	find_accessible_path(shell, cmd);
	return (check_cmd(cmd));
}
