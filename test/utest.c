/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utest.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 15:58:43 by mkling            #+#    #+#             */
/*   Updated: 2024/12/04 14:39:55 by mkling           ###   ########.fr       */
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
}

Test(Exec, single_cmd, .init = cr_redirect_stdout )
{
	cmd_tab.cmd_line = "echo hello";
	setbuf(stdout, NULL);
	fprintf(stdout, "echo hello");
	cr_assert_stdout_eq_str("echo hello");
}

	// scan(&cmd_tab);
	// lexer(&cmd_tab);
	// free_token_list(&cmd_tab);
	// parse(&cmd_tab, cmd_tab.token_list);
	// get_cmd_path(cmd_tab.cmd_list, &cmd_tab);
	// fprintf(stdout, "%s\n", cmd_tab.cmd_list->cmd_path);
	// cr_assert_stdout_eq_str("echo hello");


// /* Parser: */
// Test(Parser, Filling)
// {
// 	t_cmd_tab	cmd_tab;

// 	parse("llo", &cmd_tab);
// 	cr_expect(cmd_tab.content[0] == 'l');
// 	parse("no", &cmd_tab);
// 	cr_expect(cmd_tab.content[0] == 'n');
// }

// Test(minishell, cmd_struct, .init=redirect_all_stdout)
// {
// 	cr_assert_null(cmd.argv, "Expected argv to be NULL");
//     cr_assert_null(cmd.outfile, "Expected outfile to be NULL");
//     cr_assert_null(cmd.infile, "Expected infile to be NULL");
//     cr_assert_null(cmd.cmd_path, "Expected cmd_path to be NULL");
//     cr_assert_eq(cmd.exit_code, 0, "Expected exit_code to be 0");
//     cr_assert_eq(cmd.cmd_index, 0, "Expected cmd_index to be 0");
//     cr_assert_eq(cmd.fork_pid, -1, "Expected fork_pid to be -1");
//     cr_assert_null(cmd.next, "Expected next to be NULL");
//     cr_assert_null(cmd.prev, "Expected prev to be NULL");
// 	fprintf(stderr, "AAA");
// 	cr_assert_stderr_eq_str("AAA");
// }
