/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 13:51:38 by mkling            #+#    #+#             */
/*   Updated: 2024/12/16 15:57:41 by mkling           ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

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

void	parse_and_exec_cmd(t_shell *shell, char *input)
{
	shell->cmd_line = input;
	lexer(shell);
	parser(shell);
	process_ast(shell, shell->ast_root);
	free_ast(&shell->ast_root);
	execute_all_cmd(shell);
	ft_lstclear(&shell->cmd_list, free_cmd);
}

void	init_readline(t_shell *shell)
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
			parse_and_exec_cmd(shell, input);
			free(input);
		}
	}
	rl_clear_history();
}
