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

void	tokenize_args(const char *input, t_list *tokens, int i)
{
	int	start;
	int	len;
	int	nbr;

	start = i + 2;
	nbr = 1;
	i = i + 2;
	while (input[i] && nbr > 0)
	{
		if (input[i] == '(')
			nbr++;
		else if (input[i] == ')')
			nbr--;
		i++;
	}
	if (nbr != 0)
	{
		error_msg("open bracket");
		free_tokens(tokens);
		return ;
	}
	len = i - start - 1;
	ft_lstadd_back(&tokens, ft_lstnew(ft_strndup(input, len, start)));
}

void	tokenize_quotes(const char *input, t_list *tokens, int i)
{
	char	quote;
	int		start;
	int		len;

	len = 0;
	start = 0;
	quote = input[i];
	i++;
	start = i;
	while (input[i] && input[i] != quote)
		i++;
	if (input[i] == quote)
	{
		len = i - start;
		ft_lstadd_back(&tokens, ft_lstnew(ft_strndup(input, len, start)));
		i++;
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
	t_list	*tokens;
	int		i;
	int		start;
	int		len;

	tokens = NULL;
	i = 0;
	if (!input)
		return (NULL);
	while (input[i])
	{
		while (input[i] && input[i] == ' ' || input[i] == '\t')
			i++;
		if (!input[i])
			break ;
		if (input[i] == '"' || input[i] == '\'')
			tokenize_quotes(input, tokens, i);
		else if (is_special_token(input + i))
		{
			len = is_special_token(input + i);
			ft_lstadd_back(&tokens, ft_lstnew(ft_strndup(input, len, start)));
			i = i + len;
		}
		else if (input[i] == '$' && input[i + 1] == '(')
			tokenize_args(input, tokens, i);
		else
		{
			start = i;
			while (input[i] && input[i] != ' ' && input[i] != '\t' && !is_special_token(input))
				i++;
			len = i - start;
			ft_lstadd_back(&tokens, ft_lstnew(ft_strndup(input, len, start)));
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
