/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skassimi <skassimi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 09:23:00 by skassimi          #+#    #+#             */
/*   Updated: 2024/12/14 13:09:03 by skassimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	if (n == 0)
		return (0);
	while (i <= n && s1[i] != '\0' && s1[i] == s2[i])
		i++;
	if (i == n)
		return (0);
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

void	free_tokens(t_list *head)
{
	t_list	*current;
	t_list	*next;

	while (current)
	{
		next = current->next;
		free(current->content);
		free(current);
		current = next;
	}
}

int	is_special_token(const char *str)
{
	const char	*special_tokens[] = {"|", ">", ">>", "<", NULL};
	int			i;

	i = 0;
	while (special_tokens[i])
	{
		if (ft_strncmp(str, special_tokens[i], ft_strlen(special_tokens[i])))
			return (ft_strlen(special_tokens[i]));
		i++;
	}
	return (0);
}

t_list	*ft_lstnew(void *content)
{
	t_list	*node;

	node = malloc(sizeof(t_list));
	if (!node)
		return (NULL);
	node->content = content;
	node->next = NULL;
	node->prev = NULL;
	return (node);
}

void	tokenize_args(const char **input, t_list **tokens)
{
	const  char *start;
	int	nbr;

	start = *input + 2;
	*input += 2;
	nbr = 1;
	while (**input && nbr > 0)
	{
		if (**input == '(')
			nbr++;
		else if (**input == ')')
			nbr--;
		(*input)++;
	}
	if (nbr != 0)
	{
		error_msg("open bracket");
		free_tokens(tokens);
		return ;
	}
	ft_lstadd_back(&tokens, ft_lstnew(ft_strndup(start, *input - start)));
}

void	tokenize_quotes(const char **input, t_list **tokens)
{
	char	quote;
	const char *start;

	*start = ++(*input);
	quote = **input;
	while (**input && **input != quote)
		(*input)++;
	if (**input == quote)
	{
		ft_lstadd_back(&tokens, ft_lstnew(ft_strndup(start, *input - start)));
		(*input)++;
	}
	else
	{
		error_msg("open quote");
		free_tokens(tokens);
		return ;
	}
}

t_list	*tokenizer(const char *input)
{
	t_list	*tokens;;
	const char	start;
	int len;

	len = 0;
	*tokens = NULL;
	i = 0;
	if (!input)
		return (NULL);
	while (*input)
	{
		while (*input && *input == ' ' || *input == '\t')
			*input++;
		if (!*input)
			break ;
		if (*input == '"' || *input == '\'')
			tokenize_quotes(&input, &tokens);
		else if (is_special_token(input))
		{
			len = is_special_token(input);
			ft_lstadd_back(&tokens, ft_lstnew(ft_strndup(input, len)));
			*input += len;
		}
		else if (*input == '$' && *(input + 1) == '(')
			tokenize_args(&input, tokens);
		else
		{
			*start = input;
			while (*input && *input != ' ' && *input != '\t' && !is_special_token(input))
				*input++;
			ft_lstadd_back(&tokens, ft_lstnew(ft_strndup(start, input - start)));
		}
	}
	return (tokens);
}

void print_tokens(t_list *tokens) {
    while (tokens) {
        printf("Token : %s\n", tokens->content);
        tokens = tokens->next;
    }
}


int main(int argc, char **argv)
{
	if (argc < 2) 
	{
        return (0);
	}
    char *input = argv[1];
    t_list *tokens = tokenizer(input);
    if (!tokens) {
        printf("Erreur : impossible de tokenizer l'entrée\n");
        return (0);
    }

    printf(" Tokens\n");
    print_tokens(tokens);
    free_tokens(tokens);
    return (0);
};
/*error_msg*/
