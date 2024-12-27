/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skassimi <skassimi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 09:23:00 by skassimi          #+#    #+#             */
/*   Updated: 2024/12/27 13:02:22 by skassimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int is_not_space(char c)
{
	if (c == ' ' || c == '\t')
		return (0);
	return (1);
}
void	free_tokens(t_list *current)
{
	t_list	*next;

	while (current)
	{
		next = current->next;
		free(current->content);
		free(current);
		current = next;
	}
}

int	is_special_token(char c)
{
	return(c == '>' || c == '<' || c == '|' || c == '$');
}

void	tokenize_special(char **input, t_list **tokens)
{
	if ((**input == '>' && *(*input + 1) == '>') || (**input == '<' && *(*input + 1) == '<'))
	{
		ft_lstadd_back(tokens, ft_lstnew(ft_strndup(*input, 2)), T_SPECIAL);
		*input += 3;
	}
	else if (**input == '$' && *(*input + 1) == '?')
	{
		ft_lstadd_back(tokens, ft_lstnew(ft_strndup(*input, 2)), T_EXIT);
		*input += 3;
	}
	else if (**input == '$' && ft_isalpha(*(*input + 1)))
	{
		ft_lstadd_back(tokens, ft_lstnew(ft_strndup(*input, 2)), T_ENV_VAR);
		*input += 3;
	}
	else
	{
		ft_lstadd_back(tokens, ft_lstnew(ft_strndup(*input, 1)), T_SPECIAL);
		*input += 2;
	}
}

void	tokenize_env(char **input, t_list **tokens)
{
	char *start;
	int len;

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
	int len;
	char	quote;
	char *start;

	(*input)++;
	start = *input; 
	quote = **input;
	len = 0;
	while (**input && **input != quote)
	{
		(*input)++;
		len++;
	}
	if (**input == quote)
	{
		printf("token_quote :%s", ft_strndup(start, len));
		ft_lstadd_back(tokens, ft_lstnew(ft_strndup(start, len)), T_WORD);
		(*input)++;
	}
	else
	{	ft_lstnew(ft_strndup(*input, 2));
		//error_msg("open quote");
		free_tokens(*tokens);
		return ;
	}
}

void tokenize_word(char **input, t_list **tokens)
{
	int len;
	char *start;
	
	len = 0;
	start = *input;
	while (**input && is_not_space(**input) && !is_special_token(**input))
	{
		//fprintf(stderr,"input while tokenize = %s\n", *input);
		(*input)++;
		len++;
	}
	ft_lstadd_back(tokens, ft_lstnew(ft_strndup(start, len)), T_WORD);
	fprintf(stderr,"input after tokenize = %s\n", *input);
}

t_list	*tokenizer(char *input)
{
	t_list	*tokens;
	
	tokens = NULL;
	if (!input)
		return (NULL);
	while (*input )
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
void print_tokens(t_list *tokens)
 
{
    while (tokens && tokens->content != NULL) {
        printf("Token : %s\n", tokens->content);
        tokens = tokens->next;
    }
}
/*error_msg*/
