/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 18:04:03 by alex              #+#    #+#             */
/*   Updated: 2024/11/30 01:32:38 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_special_char(int c)
{
	if (ft_strchr(DELIMITERS, c) || ft_strchr(OPERATORS, c)
		|| ft_strchr(WHITESPACE, c))
		return (1);
	return (0);
}

void	id_delimiter(t_cmd_tab *cmd_tab)
{
	char	*content;

	content = ft_substr(cmd_tab->cmd_line, cmd_tab->index, 1);
	add_token(cmd_tab, DELIMITER, content);
	cmd_tab->index++;
}

void	id_operator(t_cmd_tab *cmd_tab)
{
	char	*content;

	content = ft_substr(cmd_tab->cmd_line, cmd_tab->index, 1);
	add_token(cmd_tab, OPERATOR, content);
	cmd_tab->index++;
}

void	id_whitespace(t_cmd_tab *cmd_tab)
{
	int		index;
	char	*word;

	index = 0;
	while (ft_strchr(WHITESPACE, cmd_tab->cmd_line[cmd_tab->index + index]))
		index++;
	word = ft_substr(cmd_tab->cmd_line, cmd_tab->index, index);
	add_token(cmd_tab, DELIMITER, word);
	cmd_tab->index += index;
}

void	id_word(t_cmd_tab *cmd_tab)
{
	int		word_index;
	char	*word;

	word_index = 0;
	while (ft_isprint(cmd_tab->cmd_line[cmd_tab->index + word_index])
		&& !is_special_char(cmd_tab->cmd_line[cmd_tab->index + word_index]))
		word_index++;
	word = ft_substr(cmd_tab->cmd_line, cmd_tab->index, word_index);
	add_token(cmd_tab, WORD, word);
	cmd_tab->index += word_index;
}
