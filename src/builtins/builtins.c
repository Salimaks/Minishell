/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 10:22:40 by alex              #+#    #+#             */
/*   Updated: 2024/12/26 14:43:56 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(t_cmd *cmd)
{
	char	*argv0;

	if (!cmd || !cmd->arg_list)
		return (0);
	argv0 = (char *)cmd->arg_list->content;
	if (ft_strcmp(argv0, "cd") == 0)
		return (1);
	if (ft_strcmp(argv0, "echo") == 0)
		return (1);
	if (ft_strcmp(argv0, "env") == 0)
		return (1);
	if (ft_strcmp(argv0, "export") == 0)
		return (1);
	if (ft_strcmp(argv0, "unset") == 0)
		return (1);
	if (ft_strcmp(argv0, "pwd") == 0)
		return (1);
	if (ft_strcmp(argv0, "exit") == 0)
		return (1);
	return (0);
}

int	exec_builtin(t_shell *shell, t_cmd *cmd)
{
	if (ft_strcmp(cmd->argv[0], "cd") == 0)
		return (cd(shell, cmd->argv[1]));
	if (ft_strcmp(cmd->argv[0], "echo") == 0)
		return (echo(cmd->argv, cmd->fd_out));
	if (ft_strcmp(cmd->argv[0], "env") == 0)
		return (env(shell, cmd->fd_out));
	if (ft_strcmp(cmd->argv[0], "export") == 0)
		return (export(shell, cmd->argv, cmd->fd_out));
	if (ft_strcmp(cmd->argv[0], "unset") == 0)
		return (unset(shell, cmd->argv));
	if (ft_strcmp(cmd->argv[0], "pwd") == 0)
		return (pwd(cmd->fd_out));
	else
		exit_shell(shell, cmd->argv);
	return (1);
}
