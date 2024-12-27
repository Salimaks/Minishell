/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skassimi <skassimi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 09:23:00 by skassimi          #+#    #+#             */
/*   Updated: 2024/12/24 14:10:52 by skassimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	if ((**input = '>' && *(*input + 1) == '>') || (**input = '<' && *(*input + 1) == '<'))
	{
		ft_lstadd_back(tokens, ft_lstnew(ft_strndup(*input, 2)), T_SPECIAL);
		*input += 2;
	}
	
}

void	tokenize_args(char **input, t_list **tokens)
{
	char *start;
	int	nbr;

	start = *input + 2;
	*input += 2;
	nbr = 1;
	while (*input && nbr > 0)
	{
		if (**input == '(')
			nbr++;
		else if (**input == ')')
			nbr--;
		(*input)++;
	}
	if (nbr != 0)
	{
		//error_msg("open bracket");
		free_tokens(*tokens);
		return ;
	}
	ft_lstadd_back(tokens, ft_lstnew(ft_strndup(start, *input - start)));
}

int	tokenize_quotes(char *input, t_list **tokens)
{
	int i;
	char	quote;
	char *start;

	(*input)++;
	start = *input;
	quote = **input;
	while (**input && **input != quote)
		(*input)++;
	if (**input == quote)
	{
		printf("token_quote :%s", ft_strndup(start, *input - start));
		ft_lstadd_back(tokens, ft_lstnew(ft_strndup(start, *input - start)));
		(*input)++;
	}
	else
	{
		//error_msg("open quote");
		free_tokens(*tokens);
		return ;
	}
}

t_list	*tokenizer(char *input)
{
	t_list	*tokens;;
	char	*start;
	int len;

	len = 0;
	tokens = NULL;
	if (!input)
		return (NULL);
	while (*input)
	{
		while (*input && (*input == ' ' || *input == '\t'))
			(*input)++;
		if (!*input)
			break ;
		printf("input : %s\n", input);
		if (*input == '"' || *input == '\'')
			tokenize_quotes(input, &tokens);
		else if (is_special_token(input))
		{
			len = is_special_token(input);
			ft_lstadd_back(&tokens, ft_lstnew(ft_strndup(input, len)));
			input += len;
		}
		else if (*input == '$' && *(input + 1) == '(')
			tokenize_args(&input, &tokens);
		else
		{
			start = input;
			while (*input && *input != ' ' && *input != '\t' && !is_special_token(input))
				input++;
			ft_lstadd_back(&tokens, ft_lstnew(ft_strndup(start, input - start)));
		}
	}
	return (tokens);
}
void print_tokens(t_list *tokens)
 
{
    while (tokens) {
        printf("Token : %s\n", tokens->content);
        tokens = tokens->next;
    }
}
/*error_msg*/
