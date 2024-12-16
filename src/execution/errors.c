/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 14:37:02 by mkling            #+#    #+#             */
/*   Updated: 2024/12/16 16:07:20 by mkling           ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "minishell.h"

void	set_error(int err_code, t_shell *shell, char *err_message)
{
	ft_putstr_fd(err_message, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	shell->critical_er = err_code;
}

void	set_error_if(int condition, int err_code, t_shell *shell,
		char *err_message)
{
	if (condition)
		set_error(err_code, shell, err_message);
}

int	catch_error(t_shell *shell)
{
	return (shell->critical_er);
}

void	fork_exit_if(int condition, int err_code, t_cmd *cmd, char *err_message)
{
	if (condition)
	{
		perror(err_message);
		cmd->exit_code = err_code;
		exit(err_code);
	}
}

int	get_last_cmd_exit_code(t_shell *shell)
{
	return (((t_cmd *)ft_lstlast(shell->cmd_list)->content)->exit_code);
}
