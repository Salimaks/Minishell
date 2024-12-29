/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skassimi <skassimi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 09:23:00 by skassimi          #+#    #+#             */
/*   Updated: 2024/12/29 11:41:35 by skassimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_special_token(char c)
{
	return (c == '>' || c == '<' || c == '|' || c == '$');
}

void	tokenize_env(char **input, t_list **tokens)
{
	char	*start;
	int		len;

	start = NULL;
	len = 0;
	*start = *(*input)++;
	(*input)++;
	while (*input && is_not_space(**input))
		(*input)++;
	ft_lstadd_back(tokens, ft_lstnew(ft_strndup(start, len)), T_ARG);
}

void	tokenize_quotes(char **input, t_list **tokens)
{
	int		len;
	char	quote;
	char	*start;

	start = *input;
	quote = **input;
	len = 0;
	while (**input && *(*input)++ != quote)
		len++;
	if (**input == quote)
	{
		printf("token_quote :%s", ft_strndup(start, len));
		ft_lstadd_back(tokens, ft_lstnew(ft_strndup(start, len)), T_WORD);
		(*input)++;
	}
	else
	{
		ft_lstnew(ft_strndup(*input, 2));
		perror("open quote");
		free_tokens(*tokens);
		return ;
	}
}

void	tokenize_word(char **input, t_list **tokens)
{
	int		len;
	char	*start;

	len = 0;
	start = *input;
	while (**input && is_not_space(**input) && !is_special_token(**input))
	{
		(*input)++;
		len++;
	}
	ft_lstadd_back(tokens, ft_lstnew(ft_strndup(start, len)), T_WORD);
}

t_list	*tokenizer(char *input)
{
	t_list	*tokens;

	tokens = NULL;
	if (!input)
		return (NULL);
	while (*input)
	{
		while (*input && !is_not_space(*input))
			input++;
		if (!*input)
			break ;
		if (*input == '"' || *input == '\'')
			tokenize_quotes(&input, &tokens);
		else if (is_special_token(*input))
			tokenize_special(&input, &tokens);
		else
			tokenize_word(&input, &tokens);
	}
	return (tokens);
}
