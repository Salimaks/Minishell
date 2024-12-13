/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utest.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 15:58:43 by mkling            #+#    #+#             */
/*   Updated: 2024/12/13 15:55:39 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <criterion/criterion.h>
#include <criterion/redirect.h>

void	redirect_all_stdout(void)
{
	cr_redirect_stdout();
	cr_redirect_stderr();
}

t_shell	shell = {
	.cmd_line = NULL,
	.token_list = NULL,
	.cmd_list = NULL,
	.cmd_count = 1,
	.index = 0,
	.env = NULL,
	.paths = NULL,
	.critical_er = 0,
};

/* PARSING: Scan */
Test(Parsing, single_cmd)
{
	shell.cmd_line = "echo hello";
	scan(&shell);
	cr_assert(shell.token_list->lexem == WORD);
	cr_assert(shell.token_list->next->lexem == BLANK);
	cr_assert(shell.token_list->next->next->lexem == WORD);
	cr_assert(shell.token_list->next->next->next->lexem == END);
	lexer(&shell);
	cr_assert(shell.token_list->lexem == WORD);
	cr_assert(shell.token_list->next->lexem == WORD);
	cr_assert(shell.token_list->next->next->lexem == END);
	parse(&shell, shell.token_list);
	cr_assert(shell.cmd_list);
	cr_assert(shell.cmd_list->argv);
	cr_assert_str_eq(shell.cmd_list->argv[0], "echo");
	cr_assert_str_eq(shell.cmd_list->argv[1], "hello");
	free_token_list(&shell);
	cr_assert(!shell.token_list);
	free_cmd_list(&shell);
}

Test(Parsing, pipe)
{
	shell.cmd_line = "ls -a | grep a";
	scan(&shell);
	cr_assert(shell.token_list->lexem == WORD);
	cr_assert(shell.token_list->next->lexem == BLANK);
	cr_assert(shell.token_list->next->next->next->next->lexem == OPERATOR);
	lexer(&shell);
	cr_assert(shell.token_list->lexem == WORD);
	cr_assert(shell.token_list->next->lexem == WORD);
	cr_assert(shell.token_list->next->next->lexem == OPERATOR);
	parse(&shell, shell.token_list);
	cr_assert(shell.cmd_list);
	cr_assert(shell.cmd_list->next);
	cr_assert(shell.cmd_list->argv);
	cr_assert(shell.cmd_list->next->argv);
	cr_assert_str_eq(shell.cmd_list->argv[0], "ls");
	cr_assert_str_eq(shell.cmd_list->next->argv[0], "grep");
	free_token_list(&shell);
	cr_assert(!shell.token_list);
}

Test(Parsing, string)
{
	shell.cmd_line = "echo \"hello and goodbye\"";
	scan(&shell);
	cr_assert(shell.token_list->lexem == WORD);
	cr_assert(shell.token_list->next->lexem == BLANK);
	cr_assert(shell.token_list->next->next->lexem == DELIMITER);
	lexer(&shell);
	cr_assert(shell.token_list->lexem == WORD);
	cr_assert(shell.token_list->next->lexem == STRING);
	cr_assert(shell.token_list->next->next->lexem == END);
	parse(&shell, shell.token_list);
	cr_assert(shell.cmd_list);
	cr_assert(shell.cmd_list->argv);
	cr_assert_str_eq(shell.cmd_list->argv[0], "echo");
	cr_assert_str_eq(shell.cmd_list->argv[1], "hello and goodbye");
	free_token_list(&shell);
	cr_assert(!shell.token_list);
}

// Test(Exec, single_cmd, .init = redirect_all_stdout )
// {
// 	// shell.cmd_line = "echo hello";
// 	// setbuf(stdout, NULL);
// 	// get_cmd_path(shell.cmd_list, &shell);
// 	// fprintf(stderr, "%s", shell.cmd_list->cmd_path);
// 	// cr_assert_stderr_eq_str("hello");
// }
// /* Parser: */
// Test(Parser, Filling)
// {
// 	t_shell	shell;
// 	parse("llo", &shell);
// 	cr_expect(shell.content[0] == 'l');
// 	parse("no", &shell);
// 	cr_expect(shell.content[0] == 'n');
// }
