/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skassimi <skassimi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 10:22:13 by skassimi          #+#    #+#             */
/*   Updated: 2025/01/09 16:43:23 by skassimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_not_space(char c)
{
	if (c == ' ' || c == '\t')
		return (0);
	return (1);
}

char	*ft_strndup(const char *src, int size)
{
	char	*new;
	int		i;

	new = malloc((size + 1) * sizeof(char));
	if (new == NULL)
		return (NULL);
	i = 0;
	while (src[i] && i <= size)
	{
		new[i] = src[i];
		i++;
	}
	new[i] = '\0';
	return (new);
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

void    print_tokens(t_list *tokens)
{
    while (tokens && tokens->content != NULL)
    {
        printf("Token : %s\n", tokens->content);
        tokens = tokens->next;
    }
}

void	tokenize_special(char **input, t_list **tokens)
{
	if ((**input == '>' && *(*input + 1) == '>') || (**input == '<' && *(*input + 1) == '<'))
	{
		ft_lstadd_back(tokens, ft_lstnew(ft_strndup(*input, 2)), OPERATOR);
		*input += 3;
	}
	else if (**input == '$' && *(*input + 1) == '?')
	{
		ft_lstadd_back(tokens, ft_lstnew(ft_strndup(*input, 2)), END);
		*input += 3;
	}
	else if (**input == '$' && ft_isalpha(*(*input + 1)))
	{
		ft_lstadd_back(tokens, ft_lstnew(ft_strndup(*input, 0)), OPERATOR);
		*input += 1;
	}
	else
	{
		ft_lstadd_back(tokens, ft_lstnew(ft_strndup(*input, 1)), DELIMITER);
		*input += 2;
	}
}
