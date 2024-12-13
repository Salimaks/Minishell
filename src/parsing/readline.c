/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 13:51:38 by mkling            #+#    #+#             */
/*   Updated: 2024/12/13 15:55:39 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_tokens(t_list *node)
{
	while (node != NULL)
	{
		printf("TOKEN %d :", ((t_token *)node->content)->lexem);
		if (node->content != NULL)
			printf("%s", ((t_token *)node->content)->content);
		printf("\n");
		node = node->next;
	}
}

void	parse_and_exec_cmd(char *input, char **env)
{
	t_shell	*shell;

	shell->cmd_line = input;
	lexer(shell);
	shell->ast_root = parse_pipe(shell, &shell->token_list);
	ft_lstclear(&shell->token_list, free_token);
	if (shell->cmd_count > 0)
		execute_all_cmd(shell);
}

void	init_readline(char **envp)
{
	char		*input;

	signals();
	while (1)
	{
		input = readline("shell> ");
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
