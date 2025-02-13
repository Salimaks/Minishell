/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skassimi <skassimi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 09:23:00 by skassimi          #+#    #+#             */
/*   Updated: 2025/02/13 12:51:52 by skassimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_special_token(char *str)
{
	if (str && *str && ft_strlen(str) >= 2)
	{
		if (!ft_strncmp(str, "<<", 2))
			return (HEREDOC);
		if (!ft_strncmp(str, ">>", 2))
			return (APPEND);
	}
	if (*str && ft_strlen(str) >= 1)
	{
		if (!ft_strncmp(str, "<", 1))
			return (INFILE);
		if (!ft_strncmp(str, ">", 1))
			return (OUTFILE);
		if (!ft_strncmp(str, "|", 1))
			return (PIPE);
	}
	return (0);
}

/*void	tokenize_env(char **input, t_list **tokens)
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
}*/

void	tokenize_quotes(char **input, t_list **tokens)
{
	int		len;
	char	quote;
	char	*start;

	quote = **input;
	len = 0;
	(*input) ++;
	start = *input;
	while ((*input)++ && **input != quote)
		len++;
	printf("token_quote 1:%s\n", ft_strndup(start, len));
	if (**input == quote)
	{
		printf("token_quote :%s\n", ft_strndup(start, len));
		ft_lstadd_back(tokens, ft_lstnew(ft_strndup(start, len)), WORD);
		(*input)++;
	}
	else
	{
		perror("open quote");
		free_tokens(*tokens);
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
	ft_lstadd_back(tokens, ft_lstnew(ft_strndup(start, len)), WORD);
}

t_list	*tokenizer(t_shell *shell, char *input)
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
	shell->token_list = tokens;
	shell->cmd_line = input;
	return (tokens);
}
