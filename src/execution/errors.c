/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 14:37:02 by mkling            #+#    #+#             */
/*   Updated: 2024/12/11 19:44:07 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_error(int err_code, t_cmd_tab *cmd_tab, char *err_message)
{
	perror(err_message);
	cmd_tab->critical_er = err_code;
}

void	set_error_if(int condition, int err_code, t_cmd_tab *cmd_tab,
		char *err_message)
{
	if (condition)
	{
		perror(err_message);
		cmd_tab->critical_er = err_code;
	}
}

int	catch_error(t_cmd_tab *cmd_tab)
{
	return (cmd_tab->critical_er);
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
