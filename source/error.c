/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 17:41:32 by mkling            #+#    #+#             */
/*   Updated: 2024/11/27 17:54:23 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/minishell.h"

void	fork_exit_if(int condition, int err_code, t_cmd *cmd, char *err_message)
{
	if (condition)
	{
		ft_putstr_fd(err_message);
		ft_putstr_fd("\n");
		cmd->exit_code = err_code;
		exit(err_code);
	}
}
