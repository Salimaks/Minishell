/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 13:06:39 by alex              #+#    #+#             */
/*   Updated: 2024/12/02 18:29:52 by mkling           ###   ########.fr       */
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

char	**create_argv(t_cmd_tab *cmd_tab, t_token *start)
{
	int		index;
	t_token	*current;
	char	**cmd_argv;

	cmd_argv = malloc(sizeof(char *)
			* (get_token_count(start, find_token_in_list(start, '\0')) + 1));
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

void	load_cmd(t_cmd_tab *cmd_tab, t_token *start)
{
	t_cmd	*new_cmd;

	new_cmd = create_cmd();
	if (!new_cmd)
	{
		set_error(MALLOC_FAIL, cmd_tab,
			"Failed to allocate command structure");
		return ;
	}
	append_cmd(new_cmd, cmd_tab);
	new_cmd->argv = create_argv(cmd_tab, start);
	new_cmd->outfile = NULL;
	new_cmd->infile = NULL;
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
		load_cmd(cmd_tab, start);
}
