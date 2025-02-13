/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skassimi <skassimi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 10:22:13 by skassimi          #+#    #+#             */
/*   Updated: 2025/02/13 13:10:05 by skassimi         ###   ########.fr       */
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

/*void    print_tokens(t_list *tokens)
{
    while (tokens && tokens->content != NULL)
    {
        printf("Token : %s\n", tokens->content);
        tokens = tokens->next;
    }
}*/

void	tokenize_special(char **input, t_list **tokens)
{
	int	type;

	type = is_special_token(*input);
	if (!type)
		return (false);
	if (type == INFILE && !ft_lstadd_back(tokens, ft_lstnew(ft_strdup("<")), INFILE))
		return (false);
	else if (type == HEREDOC && !ft_lstadd_back(tokens, ft_lstnew(ft_strdup("<<")), HEREDOC))
		return (false);
	else if (type == OUTFILE && !ft_lstadd_back(tokens, ft_lstnew(ft_strdup(">")), OUTFILE))
		return (false);
	else if (type == APPEND && !ft_lstadd_back(tokens, ft_strdup(">>"), APPEND))
		return (false);
	else if (type == PIPE && !ft_lstadd_back(tokens, ft_strdup("|"), PIPE))
		return (false);
	if (type == INFILE || type == OUTFILE || type == PIPE)
		(*command)++;
	else if (type == HEREDOC || type == APPEND)
		(*command) += 2;
	return (true);
}
