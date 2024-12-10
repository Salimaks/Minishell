/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 13:06:39 by alex              #+#    #+#             */
/*   Updated: 2024/12/10 17:57:03 by alex             ###   ########.fr       */
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

size_t	count_tokens(t_token *start, int lexem)
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

char	**id_infiles(t_cmd_tab *cmd_tab, t_token *start)
{
	int		index;
	int		infile_count;
	t_token	*current;
	char	**infile_list;

	infile_count = count_tokens(start, LESSER);
	if (infile_count == 0)
		return (NULL);
	infile_list = ft_calloc(sizeof(char *), infile_count + 1);
	if (!infile_list)
		return (set_error(MALLOC_FAIL, cmd_tab,
			"Failed to allocate infile name"), NULL);
	index = 0;
	while (index < infile_count)
	{
		current = find_token_in_list(start, LESSER);
		current = current->next;
		pop_token(current->prev);
		infile_list[index] = ft_strdup(current->content);
		current = current->next;
		pop_token(current->prev);
		index++;
	}
	infile_list[index] = NULL;
	return (infile_list);
}

char	**id_outfiles(t_cmd_tab *cmd_tab, t_token *start)
{
	int		index;
	int		outfiles_count;
	t_token	*current;
	char	**outfiles_list;

	outfiles_count = count_tokens(start, GREATER);
	if (outfiles_count == 0)
		return (NULL);
	outfiles_list = ft_calloc(sizeof(char *), outfiles_count + 1);
	if (!outfiles_list)
		return (set_error(MALLOC_FAIL, cmd_tab,
			"Failed to allocate infile name"), NULL);
	index = 0;
	while (index < outfiles_count)
	{
		current = find_token_in_list(start, GREATER);
		current = current->next;
		pop_token(current->prev);
		outfiles_list[index] = ft_strdup(current->content);
		current = current->next;
		pop_token(current->prev);
		index++;
	}
	outfiles_list[index] = NULL;
	return (outfiles_list);
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
	new_cmd->outfile = id_outfiles(cmd_tab, start);
	new_cmd->infile = id_infiles(cmd_tab, start);
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
