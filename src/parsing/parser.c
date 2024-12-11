/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 13:06:39 by alex              #+#    #+#             */
/*   Updated: 2024/12/11 16:49:36 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	get_token_count(t_token *start, t_token *end)
{
	t_token	*current;
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

size_t	count_tokens_of_type(t_token *start, int lexem)
{
	t_token	*current;
	size_t	count;

	current = start;
	count = 0;
	while (current->lexem != END)
	{
		if (current->lexem == lexem)
			count++;
		current = current->next;
	}
	return (count);
}

char	**extract_as_array(t_cmd_tab *cmd_tab, t_token *start, int type)
{
	int		index;
	int		count;
	t_token	*current;
	char	**result;

	count = count_tokens_of_type(start, type);
	if (count == 0)
		return (NULL);
	result = ft_calloc(sizeof(char *), count + 1);
	if (!result)
		return (set_error(MALLOC_FAIL, cmd_tab,
				"Failed to allocate infile path"), NULL);
	index = 0;
	current = start;
	while (current->lexem != END)
	{
		if (current->lexem == type)
			result[index++] = ft_strdup(current->content);
		current = current->next;
	}
	result[index] = NULL;
	return (result);
}

void	parse_cmd(t_cmd_tab *cmd_tab, t_token *start)
{
	t_cmd	*new_cmd;

	new_cmd = create_cmd(cmd_tab);
	if (!new_cmd)
		return (set_error(MALLOC_FAIL, cmd_tab,
				"Failed to allocate command structure"));
	new_cmd->outfile = extract_as_array(cmd_tab, start, OUTFILE);
	new_cmd->infile = extract_as_array(cmd_tab, start, INFILE);
	new_cmd->argv = extract_as_array(cmd_tab, start, WORD);
	cmd_tab->cmd_count++;
}

void	parse(t_cmd_tab *cmd_tab, t_token *start)
{
	t_token	*pipe;

	pipe = find_token_in_list(start, '|');
	if (pipe != NULL)
	{
		parse(cmd_tab, pipe->next);
		pipe->lexem = END;
	}
	if (start->lexem != END)
		parse_cmd(cmd_tab, start);
}
