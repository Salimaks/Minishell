/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 13:51:38 by mkling            #+#    #+#             */
/*   Updated: 2024/12/26 14:42:25 by alex             ###   ########.fr       */
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

void	parse_and_exec_cmd(t_shell *shell, char *input)
{
	shell->cmd_line = input;
	shell->critical_er = 0;
	lexer(shell);
	if (check_syntax(shell, shell->token_list) != 0)
		return (ft_lstclear(&shell->token_list, free_token));
	parser(shell);
	shell->last_exit_code = exec_tree(shell, shell->tree_root, false);
	free_tree(&shell->tree_root);
}

void	init_readline(t_shell *shell)
{
	char		*input;

	signals();
	while (1)
	{
		input = readline("shell$ ");
		if (!input)
			break ;
		if (input)
		{
			parse_and_exec_cmd(shell, input);
			add_history(input);
			free(input);
		}
	}
	rl_clear_history();
}
