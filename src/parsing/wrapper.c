/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wrapper.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 13:50:23 by alex              #+#    #+#             */
/*   Updated: 2024/12/28 13:52:06 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	token_is(int lexem, t_list *node)
{
	if (!node || !node->content)
		return (0);
	return (((t_token *)node->content)->lexem == lexem);
}

int	letter_is(int lexem, char letter)
{
	int	token_type;

	if (ft_strchr(DELIMITERS, letter) != NULL)
		token_type = DELIMITER;
	else if (ft_strchr(OPERATORS, letter))
		token_type = OPERATOR;
	else if (ft_strchr(BLANKS, letter))
		token_type = BLANK;
	else
		token_type = WORD;
	return (token_type == lexem);
}
