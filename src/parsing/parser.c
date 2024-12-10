/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 13:06:39 by alex              #+#    #+#             */
/*   Updated: 2024/12/10 20:15:06 by alex             ###   ########.fr       */
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

char	**id_redirection(t_cmd_tab *cmd_tab, t_token *start, int type)
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
			"Failed to allocate infile name"), NULL);
	index = 0;
	while (index < count)
	{
		current = find_token_in_list(start, type);
		current = current->next;
		pop_token(current->prev);
		result[index] = ft_strdup(current->content);
		current = current->next;
		pop_token(current->prev);
		index++;
	}
	result[index] = NULL;
	return (result);
}

char	**create_argv(t_cmd_tab *cmd_tab, t_token *start)
{
	int		index;
	t_token	*current;
	char	**cmd_argv;

	if (start->lexem == START)
		start = start->next;
	cmd_argv = ft_calloc(sizeof(char *),
			(get_token_count(start, find_token_in_list(start, '\0')) + 1));
	if (!cmd_argv)
	{
		set_error(MALLOC_FAIL, cmd_tab,
			"Failed to allocate command structure");
		return (NULL);
	}
	current = start;
	index = 0;
	while (current->lexem != END)
	{
		cmd_argv[index] = ft_strdup(current->content);
		current = current->next;
		index++;
	}
	cmd_argv[index] = NULL;
	return (cmd_argv);
}

void	parse_cmd(t_cmd_tab *cmd_tab, t_token *start)
{
	t_cmd	*new_cmd;

	new_cmd = create_cmd(cmd_tab);
	if (!new_cmd)
		return (set_error(MALLOC_FAIL, cmd_tab,
			"Failed to allocate command structure"));
	new_cmd->outfile = id_redirection(cmd_tab, start, GREATER);
	new_cmd->infile = id_redirection(cmd_tab, start, LESSER);
	new_cmd->argv = create_argv(cmd_tab, start);
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
