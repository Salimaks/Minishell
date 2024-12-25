/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 14:37:02 by mkling            #+#    #+#             */
/*   Updated: 2024/12/25 13:15:50 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_error(void)
{
	ft_putstr_fd(SHELL_NAME, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
}

void	set_error(int err_code, t_shell *shell, char *err_message)
{
	print_error();
	ft_putstr_fd(err_message, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	shell->critical_er = err_code;
}

void	set_cmd_error(int err_code, t_cmd *cmd, char *err_message)
{
	print_error();
	if (cmd->arg_list && (char *)cmd->arg_list->content)
	{
		ft_putstr_fd((char *)cmd->arg_list->content, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	ft_putstr_fd(err_message, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	cmd->exit_code = err_code;
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
