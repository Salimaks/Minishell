/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 13:51:38 by mkling            #+#    #+#             */
/*   Updated: 2024/11/30 00:56:44 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_tokens(t_token *first)
{
	t_token	*current;

	current = first;
	while (current)
	{
		printf("TOKEN %d : %s\n", current->type, current->content);
		current = current->next;
	}
}

void	init_readline(t_cmd_tab *cmd_tab)
{
	char	*input;

	while (1)
	{
		input = readline("Minishell> ");
		if (!input)
			break ;
		add_history(input);
		cmd_tab->cmd_line = input;
		tokenize(cmd_tab);
		print_tokens(cmd_tab->token_list);
		free_token_list(cmd_tab);
		free(input);
	}
}
