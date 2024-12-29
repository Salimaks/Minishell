/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 14:12:32 by alex              #+#    #+#             */
/*   Updated: 2024/12/29 01:20:08 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_if_directory(t_cmd *cmd, char *path)
{
	int	fd;

	fd = open(path, __O_DIRECTORY | O_RDONLY);
	if (fd < 0)
		return (set_cmd_error(CANT_EXECUTE_CMD, cmd, "Is not a directory"));
	close(fd);
	return (set_cmd_error(CANT_EXECUTE_CMD, cmd, "Is a directory"));
}

void	check_cmd(t_cmd *cmd)
{
	if (cmd->cmd_path == NULL)
		return (set_cmd_error(CANT_FIND_CMD, cmd, "Command not found"));
	// fprintf(stderr, "check cmd path is %s\n", cmd->cmd_path);
	if (access(cmd->cmd_path, X_OK) != 0)
		return (set_cmd_error(CANT_EXECUTE_CMD, cmd, "Permission denied"));
}

void	try_working_dir_path(t_shell *shell, t_cmd *cmd)
{
	char	*working_dir;

	if (!find_env(shell->env_list, "PWD"))
		return ;
	working_dir = &((char *)find_env(shell->env_list, "PWD")->content)[5];
	cmd->cmd_path = ft_strjoin(working_dir, &cmd->cmd_path[1]);
	return (check_cmd(cmd));
}

void	try_relative_paths(t_shell *shell, t_cmd *cmd)
{
	char	*tested_path;
	char	*path_env;
	size_t	i;

	i = 0;
	if (!find_env(shell->env_list, "PATH"))
		return (set_cmd_error(CANT_FIND_CMD, cmd, "No PATH variable found"));
	path_env = (char *)find_env(shell->env_list, "PATH")->content;
	shell->paths = ft_split(&path_env[5], ':');
	if (!shell->paths)
		set_error(MALLOC_FAIL, shell, "Failed to split path list");
	while (shell->paths[i])
	{
		tested_path = ft_strjoin(shell->paths[i++], cmd->cmd_path);
		if (!tested_path)
			return (set_cmd_error(MALLOC_FAIL, cmd, "Failed to allocate path"));
		if (access(tested_path, F_OK | R_OK) == 0)
		{
			cmd->cmd_path = tested_path;
			return (check_cmd(cmd));
		}
		free(tested_path);
	}
	cmd->cmd_path = NULL;
	return (set_cmd_error(CANT_FIND_CMD, cmd, "Command not found"));
}

/* Checks first absolute path for command
Then paths if any were extracted from env
Exits fork if no command is found */
void	get_cmd_path(t_shell *shell, t_cmd *cmd)
{
	if (!cmd || !cmd->arg_list || !((char *)cmd->arg_list->content)[0])
		return (set_cmd_error(CANT_FIND_CMD, cmd, "Command not found"));
	cmd->cmd_path = (char *)cmd->arg_list->content;
	if (cmd->cmd_path[ft_strlen(cmd->cmd_path) - 1] == '/')
		return (check_if_directory(cmd, cmd->cmd_path));
	if (ft_strnstr(cmd->cmd_path, "./", 2))
		return (try_working_dir_path(shell, cmd));
	cmd->cmd_path = ft_strjoin("/", cmd->cmd_path);

	if (!cmd->cmd_path)
		return (set_cmd_error(MALLOC_FAIL, cmd, "Failed to allocate path"));
	if (access(cmd->cmd_path, F_OK) == 0)
		return (check_cmd(cmd));
	return (try_relative_paths(shell, cmd));
}
