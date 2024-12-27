/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanner.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 16:37:27 by alex              #+#    #+#             */
/*   Updated: 2024/12/27 14:31:32 by mkling           ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "minishell.h"

size_t	get_token_count(t_list *start, t_list *end)
{
	t_list	*current;
	size_t	count;

	current = start;
	count = 0;
	while (current != end)
	{
		count++;
		current = current->next;
	}
	return (count);
}

size_t	count_tokens_of_type(t_list *start, int lexem)
{
	t_list	*current;
	size_t	count;

	current = start;
	count = 0;
	while (((t_token *)current->content)->lexem != END)
	{
		if (((t_token *)current->content)->lexem == lexem)
			count++;
		current = current->next;
	}
	return (count);
}

char	**extract_token_as_array(t_shell *shell, t_list *start, int type)
{
	int		index;
	int		count;
	char	**result;

	count = count_tokens_of_type(start, type);
	if (count == 0)
		return (NULL);
	result = ft_calloc(sizeof(char *), count + 1);
	if (!result)
		return (set_error(MALLOC_FAIL, shell,
				"Failed to allocate infile path"), NULL);
	index = 0;
	while (((t_token *)start->content)->lexem != END)
	{
		if (((t_token *)start->content)->lexem == type)
			result[index++] = ft_strdup(((t_token *)start->content)->content);
		start = start->next;
	}
	result[index] = NULL;
	return (result);
}

void	merge_token(t_shell *shell, t_list *start)
{
	t_token	*current;
	t_token	*next;

	current = ((t_token *)start->content);
	next = ((t_token *)start->next->content);
	current->content = ft_strjoinfree(current->content, next->content);
	if (!current->content)
		return (set_error(MALLOC_FAIL, shell, "Failed to malloc token"));
	ft_lstpop(&shell->token_list, start->next, free_token);
}

void	scan(t_shell *shell)
{
	shell->index = 0;
	add_token(shell, START, '\0');
	while (shell->index < ft_strlen(shell->cmd_line))
	{
		if (ft_strchr(DELIMITERS, shell->cmd_line[shell->index]))
			add_token(shell, DELIMITER, shell->cmd_line[shell->index]);
		else if (ft_strchr(OPERATORS, shell->cmd_line[shell->index]))
			add_token(shell, OPERATOR, shell->cmd_line[shell->index]);
		else if (ft_strchr(BLANKS, shell->cmd_line[shell->index]))
			add_token(shell, BLANK, shell->cmd_line[shell->index]);
		else
			add_token(shell, WORD, shell->cmd_line[shell->index]);
		shell->index++;
	}
	add_token(shell, END, '\n');
}
