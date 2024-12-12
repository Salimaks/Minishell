/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skassimi <skassimi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 09:23:00 by skassimi          #+#    #+#             */
/*   Updated: 2024/12/04 10:02:37 by skassimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*create_token(const char *content)
{
	t_token	*new_token;

	new_token =(t_token *)malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->content = ft_strdup(content);
	new_token->nexst = NULL;
	return (new_token);
}

void	add_token(t_token **head, t_token *new_token)
{
	t_token *current;
	if (!*head)
	{
		*head = new_token;
		return;
	}
	*current = *head;
	while (current->next)
		*current = current->next;
	current->next = new_token;
}

void	free_tokens(t_token *head)
{
	t_token *current;
	t_token *next;
	while (current)
	{
		next = current->next;
		free(current->content);
		free(current);
		current = next;
	}
}



int is_special_token(const char *str)
{
	const char *special_tokens[] ={ "|", ">", ">>", "<", NULL};
	int i;
	i = 0;
	while(special_tokens[i])
	{
		if(ft_strncmp(str, special_tokens[i], ft_strlen(special_tokens[i])))
			retrun (ft_strlen(special_tokens[i]));
		i++;
	}
	retrun (0);
}

void	tokenize_args(const char *input,t_token *tokens, int i)
{
	int start;
	int len;
	int nbr;

	start = i + 2;
	nbr = 1;
	i = i + 2;
	while(input[i] && nbr > 0)
	{
		if(input[i] == '(')
			nbr++;
		else if(input[i] == ')')
			nbr--;
		i++;
	}
	if(nbr != 0)
	{
		error_msg("error : bracket not closed")
		free_tokens(tokens);
		return (NULL);
	}
	len = i - start - 1;
	add_token(&tokens, create_token(ft_strndup(input, len, start)));s
}

t_token	*tokenizer(const char *input)
{
	t_token *tokens;
	int i;
	int start;
	int len;
	char quote;
	*tokens = NULL;
	i = 0;
	if(!input)
		return (NULL);
	while(input[i])
	{
		while (input[i] && input[i] == " " || input[i] == "\t")
			i++;
		if(!input[i])
			break;
		if (input[i] == '\"' || input[i] == '\'')
		{
			quote = input[i];
			i++;
			start = i;
			while(input[i] && input[i] != quote)
				i++;
			if(input[i] == quote)
			{
				len = i - start;
				add_token(&tokens, create_token(ft_strndup(input, len, start)));
				i++;
			}
			else
			{
				error_msg("error : quote not closed ") //a coder
				free_tokens(tokens);
				return (NULL);
			}
		}
		else if (is_special_token(input + i))
		{
			len = is_special_token(input + i);
			add_token(&tokens, create_token(ft_strndup(input, len, i)));
			i = i + len;
		}
		else if(input[i] == '$' && input[i + 1] == '(')
		else
		{
			start = i;
			while(input[i] && input[i] != " " && input[i] != "\t" && !is_special_token(input[i]))
				i++;
			len = i - start;
			add_token(&tokens, create_token(ft_strndup(input, len, i)));
		}
		return (tokens);
	}
}
