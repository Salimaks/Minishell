/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utest.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 15:58:43 by mkling            #+#    #+#             */
/*   Updated: 2024/12/04 15:42:50 by mkling           ###   ########.fr       */
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

t_cmd_tab	cmd_tab = {
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
	cmd_tab.cmd_line = "echo hello";
	scan(&cmd_tab);
	cr_assert(cmd_tab.token_list->lexem == WORD);
	cr_assert(cmd_tab.token_list->next->lexem == WHITESPACE);
	cr_assert(cmd_tab.token_list->next->next->lexem == WORD);
	cr_assert(cmd_tab.token_list->next->next->next->lexem == END);
	lexer(&cmd_tab);
	cr_assert(cmd_tab.token_list->lexem == WORD);
	cr_assert(cmd_tab.token_list->next->lexem == WORD);
	cr_assert(cmd_tab.token_list->next->next->lexem == END);
	parse(&cmd_tab, cmd_tab.token_list);
	cr_assert(cmd_tab.cmd_list);
	cr_assert(cmd_tab.cmd_list->argv);
	cr_assert_str_eq(cmd_tab.cmd_list->argv[0], "echo");
	cr_assert_str_eq(cmd_tab.cmd_list->argv[1], "hello");
	free_token_list(&cmd_tab);
	cr_assert(!cmd_tab.token_list);
	free_cmd_list(&cmd_tab);
}

Test(Parsing, pipe)
{
	cmd_tab.cmd_line = "ls -a | grep a";
	scan(&cmd_tab);
	cr_assert(cmd_tab.token_list->lexem == WORD);
	cr_assert(cmd_tab.token_list->next->lexem == WHITESPACE);
	cr_assert(cmd_tab.token_list->next->next->next->next->lexem == OPERATOR);
	lexer(&cmd_tab);
	cr_assert(cmd_tab.token_list->lexem == WORD);
	cr_assert(cmd_tab.token_list->next->lexem == WORD);
	cr_assert(cmd_tab.token_list->next->next->lexem == OPERATOR);
	parse(&cmd_tab, cmd_tab.token_list);
	cr_assert(cmd_tab.cmd_list);
	cr_assert(cmd_tab.cmd_list->next);
	cr_assert(cmd_tab.cmd_list->argv);
	cr_assert(cmd_tab.cmd_list->next->argv);
	cr_assert_str_eq(cmd_tab.cmd_list->argv[0], "ls");
	cr_assert_str_eq(cmd_tab.cmd_list->next->argv[0], "grep");
	free_token_list(&cmd_tab);
	cr_assert(!cmd_tab.token_list);
}

Test(Parsing, string)
{
	cmd_tab.cmd_line = "echo \"hello and goodbye\"";
	scan(&cmd_tab);
	cr_assert(cmd_tab.token_list->lexem == WORD);
	cr_assert(cmd_tab.token_list->next->lexem == WHITESPACE);
	cr_assert(cmd_tab.token_list->next->next->lexem == DELIMITER);
	lexer(&cmd_tab);
	cr_assert(cmd_tab.token_list->lexem == WORD);
	cr_assert(cmd_tab.token_list->next->lexem == STRING);
	cr_assert(cmd_tab.token_list->next->next->lexem == END);
	parse(&cmd_tab, cmd_tab.token_list);
	cr_assert(cmd_tab.cmd_list);
	cr_assert(cmd_tab.cmd_list->argv);
	cr_assert_str_eq(cmd_tab.cmd_list->argv[0], "echo");
	cr_assert_str_eq(cmd_tab.cmd_list->argv[1], "hello and goodbye");
	free_token_list(&cmd_tab);
	cr_assert(!cmd_tab.token_list);
}

// Test(Exec, single_cmd, .init = redirect_all_stdout )
// {
// 	// cmd_tab.cmd_line = "echo hello";
// 	// setbuf(stdout, NULL);
// 	// get_cmd_path(cmd_tab.cmd_list, &cmd_tab);
// 	// fprintf(stderr, "%s", cmd_tab.cmd_list->cmd_path);
// 	// cr_assert_stderr_eq_str("hello");
// }
// /* Parser: */
// Test(Parser, Filling)
// {
// 	t_cmd_tab	cmd_tab;
// 	parse("llo", &cmd_tab);
// 	cr_expect(cmd_tab.content[0] == 'l');
// 	parse("no", &cmd_tab);
// 	cr_expect(cmd_tab.content[0] == 'n');
// }
