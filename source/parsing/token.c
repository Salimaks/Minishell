/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 18:04:03 by alex              #+#    #+#             */
/*   Updated: 2024/11/29 18:05:38 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	id_separator(t_cmd_tab *cmd_tab)
{
	char	c;

	c = cmd_tab->cmd_line[cmd_tab->index];
	if (c != '\'' || c != '\"')
		return ;
	add_token(cmd_tab, DELIMITER);
	cmd_tab->index++;
}
