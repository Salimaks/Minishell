/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 13:06:39 by alex              #+#    #+#             */
/*   Updated: 2024/12/12 20:47:52 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

char	**extract_token_as_array(t_cmd_tab *cmd_tab, t_list *start, int type)
{
	int		index;
	int		count;
	t_list	*current;
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
	while (((t_token *)current->content)->lexem != END)
	{
		if (((t_token *)current->content)->lexem == type)
			result[index++] = ft_strdup(((t_token *)current->content)->content);
		current = current->next;
	}
	result[index] = NULL;
	return (result);
}

void	extract_files_as_linklist(t_cmd_tab *cmd_tab, t_cmd *cmd, t_list *start)
{
	t_list	*current;
	t_token	*curr_token;

	current = start;
	while (current)
	{
		curr_token = ((t_token *)current->content);
		if (curr_token->lexem == INFILE || curr_token->lexem == HEREDOC
			|| curr_token->lexem == OUTFILE || curr_token->lexem == APPEND)
			create_file(cmd_tab, cmd, curr_token);
		current = current->next;
	}
}

void	parse_cmd(t_cmd_tab *cmd_tab, t_list *start)
{
	t_cmd	*new_cmd;

	new_cmd = create_cmd(cmd_tab);
	if (!new_cmd)
		return (set_error(MALLOC_FAIL, cmd_tab,
				"Failed to allocate command structure"));
	extract_files_as_linklist(cmd_tab, new_cmd, start);
	new_cmd->argv = extract_token_as_array(cmd_tab, start, WORD);
	cmd_tab->cmd_count++;
}

void	parse(t_cmd_tab *cmd_tab, t_list *start)
{
	t_list	*pipe;

	pipe = find_token_in_list(start, '|');
	if (pipe != NULL)
	{
		parse(cmd_tab, pipe->next);
		((t_token *)pipe->content)->lexem = END;
	}
	if (((t_token *)start->content)->lexem != END)
		parse_cmd(cmd_tab, start);
}
