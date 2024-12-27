/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanner.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 16:37:27 by alex              #+#    #+#             */
/*   Updated: 2024/12/27 19:01:48 by mkling           ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "minishell.h"

int	is_blank(t_list *node)
{
	if (!node || !node->content)
		return (0);
	return (((t_token *)node->content)->lexem == BLANK);
}

void	add_word_token(t_shell *shell, t_list **dest, char *input)
{
	int		len;
	char	*word;
	t_token	*token;

	len = 0;
	while (input[len++])
	{
		if (ft_strchr(DELIMITERS, input[len])
			|| ft_strchr(OPERATORS, input[len])
			|| ft_strchr(BLANKS, input[len]))
			break ;
	}
	word = ft_calloc(sizeof(char), len + 1);
	ft_strlcat(word, input, len + 1);
	token = create_token(shell, input[shell->index], word);
	ft_lstadd_back(dest, ft_lstnew(token));
	shell->index += len;
}

void	add_blank_token(t_shell *shell, t_list **dest, char *input)
{
	int		len;
	char	*space;
	t_token	*token;

	len = 0;
	while (ft_strchr(BLANKS, input[len]))
		len++;
	space = ft_calloc(sizeof(char), len + 1);
	ft_strlcat(space, input, len + 1);
	token = create_token(shell, input[shell->index], space);
	ft_lstadd_back(dest, ft_lstnew(token));
	shell->index += len;
}

void	add_operator_token(t_shell *shell, t_list **dest, char *input)
{
	char	*content;

	if (input[shell->index] == input[shell->index + 1])
	{
		content = ft_calloc(sizeof(char), 3);
		ft_strlcat(content, input, 3);
		add_token(shell, dest, input[shell->index], content);
		shell->index += 2;
		return ;
	}
	content = NULL;
	add_token(shell, dest, input[shell->index], NULL);
	shell->index++;
}

void	scan(t_shell *shell, t_list **token_list, char *input)
{
	shell->index = 0;
	add_token(shell, token_list, '\n', NULL);
	while (shell->index < ft_strlen(input))
	{
		if (ft_strchr(DELIMITERS, input[shell->index]))
			add_token(shell, token_list, input[shell->index], NULL);
		else if (ft_strchr(OPERATORS, input[shell->index]))
			add_operator_token(shell, token_list, input);
		else if (ft_strchr(BLANKS, input[shell->index]))
			add_blank_token(shell, token_list, input);
		else
			add_word_token(shell, token_list, input);
		shell->index++;
	}
	add_token(shell, token_list, END, NULL);
}
