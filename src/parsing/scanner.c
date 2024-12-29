/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanner.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 16:37:27 by alex              #+#    #+#             */
/*   Updated: 2024/12/29 10:43:11 by mkling           ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "minishell.h"

void	add_delimiter_token(t_shell *shell, t_list **dest, char *input)
{
	t_token	*token;

	token = create_token(shell, DELIMITER, input[shell->index], NULL);
	ft_lstadd_back(dest, ft_lstnew(token));
	shell->index++;
}

void	add_word_token(t_shell *shell, t_list **dest, char *input)
{
	int		len;
	char	*word;
	t_token	*token;

	len = 0;
	while (input[len])
	{
		if (!letter_is(WORD, input[shell->index + len]))
			break ;
		len++;
	}
	word = ft_calloc(sizeof(char), len + 1);
	ft_strlcat(word, &input[shell->index], len + 1);
	token = create_token(shell, WORD, input[shell->index], word);
	ft_lstadd_back(dest, ft_lstnew(token));
	shell->index += len;
}

void	add_blank_token(t_shell *shell, t_list **dest, char *input)
{
	int		len;
	char	*space;
	t_token	*token;

	len = 0;
	while (letter_is(BLANK, input[shell->index + len]))
		len++;
	space = ft_calloc(sizeof(char), len + 1);
	ft_strlcat(space, &input[shell->index], len + 1);
	token = create_token(shell, BLANK, input[shell->index], space);
	ft_lstadd_back(dest, ft_lstnew(token));
	shell->index += len;
}

void	add_operator_token(t_shell *shell, t_list **dest, char *input)
{
	char	*content;
	t_token	*token;

	if (input[shell->index] == input[shell->index + 1])
	{
		content = ft_calloc(sizeof(char), 3);
		ft_strlcat(content, &input[shell->index], 3);
		token = create_token(shell, OPERATOR, input[shell->index], content);
		ft_lstadd_back(dest, ft_lstnew(token));
		shell->index += 2;
		return ;
	}
	content = NULL;
	token = create_token(shell, OPERATOR, input[shell->index], NULL);
	ft_lstadd_back(dest, ft_lstnew(token));
	shell->index++;
}

void	scan(t_shell *shell, t_list **dest, char *input)
{
	shell->index = 0;
	ft_lstadd_back(dest, ft_lstnew(create_token(shell, START, '\0', NULL)));
	while (shell->index < ft_strlen(input))
	{
		if (letter_is(DELIMITER, input[shell->index]))
			add_delimiter_token(shell, dest, input);
		else if (letter_is(OPERATOR, input[shell->index]))
			add_operator_token(shell, dest, input);
		else if (letter_is(BLANK, input[shell->index]))
			add_blank_token(shell, dest, input);
		else
			add_word_token(shell, dest, input);
	}
	ft_lstadd_back(dest, ft_lstnew(create_token(shell, END, '\n', NULL)));
}
