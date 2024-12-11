/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 13:51:38 by mkling            #+#    #+#             */
/*   Updated: 2024/12/11 18:38:58 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	print_tokens(t_token *first)
// {
// 	t_token	*current;

// 	current = first;
// 	while (current)
// 	{
// 		printf("TOKEN %d :", current->lexem);
// 		if (current->content != NULL)
// 			printf("%s", current->content);
// 		printf("\n");
// 		current = current->next;
// 	}
// }

void	parse_and_exec_cmd(char *input, char **env)
{
	t_cmd_tab	*cmd_tab;

	cmd_tab = create_cmd_tab(env);
	cmd_tab->cmd_line = input;
	lexer(cmd_tab);
	parse(cmd_tab, cmd_tab->token_list);
	free_token_list(cmd_tab);
	if (cmd_tab->cmd_count > 0)
	{
		execute_all_cmd(cmd_tab);
		free_cmd_list(cmd_tab);
	}
	free_cmd_tab(cmd_tab);
}

void	init_readline(char **envp)
{
	char		*input;

	signals();
	while (1)
	{
		input = readline("Minishell> ");
		if (!input)
			break ;
		if (input)
		{
			add_history(input);
			parse_and_exec_cmd(input, envp);
			free(input);
		}
	}
	rl_clear_history();
}
