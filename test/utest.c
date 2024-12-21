/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utest.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 15:58:43 by mkling            #+#    #+#             */
/*   Updated: 2024/12/21 23:11:04 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <criterion/new/assert.h>

extern char	**environ;
t_shell		*shell;

void	redirect_all_std(void)
{
	cr_redirect_stderr();
	cr_redirect_stdout();
}

/* PARSING: */

/* Lexer */




/* Syntax */
Test(Syntax, Valid_Single_Command0)
{
	t_shell	*shell;

	shell = create_minishell(environ);
	shell->cmd_line = "echo hello";
	lexer(shell);
	cr_assert(check_syntax(shell, shell->token_list) == 0);
	free_minishell(shell);
}

Test(Syntax, Valid_Single_Command1)
{
	t_shell	*shell;

	shell = create_minishell(environ);
	shell->cmd_line = "echo      hello     ";
	lexer(shell);
	cr_assert(check_syntax(shell, shell->token_list) == 0);
	free_minishell(shell);
}

Test(Syntax, Valid_Out_Redirection0)
{
	t_shell	*shell;

	shell = create_minishell(environ);
	shell->cmd_line = "echo hello > test.txt";
	lexer(shell);
	cr_assert(check_syntax(shell, shell->token_list) == 0);
	free_minishell(shell);
}

Test(Syntax, Valid_Out_Redirection1)
{
	t_shell	*shell;

	shell = create_minishell(environ);
	shell->cmd_line = "echo hello >test.txt";
	lexer(shell);
	cr_assert(check_syntax(shell, shell->token_list) == 0);
	free_minishell(shell);
}

Test(Syntax, Valid_In_Redirection0)
{
	t_shell	*shell;

	shell = create_minishell(environ);
	shell->cmd_line = "< Makefile grep a";
	lexer(shell);
	cr_assert(check_syntax(shell, shell->token_list) == 0);
	free_minishell(shell);
}

Test(Syntax, Valid_In_Redirection1)
{
	t_shell	*shell;

	shell = create_minishell(environ);
	shell->cmd_line = "<Makefile grep a";
	lexer(shell);
	cr_assert(check_syntax(shell, shell->token_list) == 0);
	free_minishell(shell);
}

Test(Syntax, Valid_Redirections0)
{
	t_shell	*shell;

	shell = create_minishell(environ);
	shell->cmd_line = "< Makefile grep a > test.txt";
	lexer(shell);
	cr_assert(check_syntax(shell, shell->token_list) == 0);
	free_minishell(shell);
}

Test(Syntax, Valid_Redirections1)
{
	t_shell	*shell;

	shell = create_minishell(environ);
	shell->cmd_line = "<Makefile grep a >test.txt";
	lexer(shell);
	cr_assert(check_syntax(shell, shell->token_list) == 0);
	free_minishell(shell);
}

Test(Syntax, Valid_Pipe0)
{
	t_shell	*shell;

	shell = create_minishell(environ);
	shell->cmd_line = "ls -a | wc -l";
	lexer(shell);
	cr_assert(check_syntax(shell, shell->token_list) == 0);
	free_minishell(shell);
}

Test(Syntax, Valid_Pipe1)
{
	t_shell	*shell;

	shell = create_minishell(environ);
	shell->cmd_line = "ls -a |wc -l";
	lexer(shell);
	cr_assert(check_syntax(shell, shell->token_list) == 0);
	free_minishell(shell);
}

Test(Syntax, Valid_Pipe2)
{
	t_shell	*shell;

	shell = create_minishell(environ);
	shell->cmd_line = "ls -a| wc -l";
	lexer(shell);
	cr_assert(check_syntax(shell, shell->token_list) == 0);
	free_minishell(shell);
}

Test(Syntax, Empty_Command)
{
	t_shell	*shell;

	shell = create_minishell(environ);
	shell->cmd_line = "";
	lexer(shell);
	cr_assert(check_syntax(shell, shell->token_list) == 0);
	free_minishell(shell);
}

Test(Syntax, Invalid_Pipe0, .init=redirect_all_std)
{
	t_shell	*shell;

	shell = create_minishell(environ);
	shell->cmd_line = "   | echo hello";
	lexer(shell);
	cr_assert(check_syntax(shell, shell->token_list) != 0);
	cr_assert_stderr_eq_str("shell: syntax error near unexpected token '|'\n");
}

Test(Syntax, Invalid_Pipe1, .init=redirect_all_std)
{
	t_shell	*shell;

	shell = create_minishell(environ);
	shell->cmd_line = "echo hello |";
	lexer(shell);
	cr_assert(check_syntax(shell, shell->token_list) != 0);
	cr_assert_stderr_eq_str("shell: syntax error near unexpected token '|'\n");
}

Test(Syntax, Invalid_Redirection1, .init=redirect_all_std)
{
	t_shell	*shell;

	shell = create_minishell(environ);
	shell->cmd_line = "<";
	lexer(shell);
	cr_assert(check_syntax(shell, shell->token_list) != 0);
	cr_assert_stderr_eq_str("shell: syntax error near unexpected token 'newline'\n");
	free_minishell(shell);
}

Test(Syntax, Invalid_Redirection2, .init=redirect_all_std)
{
	t_shell	*shell;

	shell = create_minishell(environ);
	shell->cmd_line = "< ";
	lexer(shell);
	cr_assert(check_syntax(shell, shell->token_list) != 0);
	cr_assert_stderr_eq_str("shell: syntax error near unexpected token 'newline'\n");
	free_minishell(shell);
}

Test(Execution, get_valid_cmd_path)
{
	t_shell	shell;
	t_cmd	cmd;
	char	*argv[] = {"ls", NULL};

	shell.env = environ;
	extract_paths(&shell);
	cmd.argv = argv;
	get_cmd_path(&shell, &cmd);
	cr_assert(cmd.cmd_path != NULL);
	cr_assert(eq(str, cmd.cmd_path, "/usr/bin/ls"));
}

Test(Execution, get_forbidden_infile_fd, .init=redirect_all_std)
{
	t_shell	shell;
	t_cmd	cmd;
	t_file	file;
	t_list	node;
	char	*arg0 = "ls";

	shell.critical_er = 0;
	file.path = "test/forbidden.txt";
	file.mode = INFILE;
	cmd.arg_list = ft_lstnew(arg0);
	node.content = &file;
	node.next = NULL;
	node.prev = NULL;
	cmd.infiles = &node;

	get_infile_fd(&shell, &cmd);
	// cr_assert(file.fd < 0);
	cr_assert_stderr_eq_str("shell: ls: Input file cannot be read\n");
}

Test(Execution, get_forbidden_outfile_fd, .init=redirect_all_std)
{
	t_cmd	cmd;
	t_file	file;
	t_list	node;
	char	*arg0 = "ls";

	file.path = "test/forbidden.txt";
	file.mode = OUTFILE;
	cmd.arg_list = ft_lstnew(arg0);
	node.content = &file;
	node.next = NULL;
	node.prev = NULL;
	cmd.outfiles = &node;

	get_outfile_fd(&cmd);
	// cr_assert(file.fd < 0);
	cr_assert_stderr_eq_str("shell: ls: Output file cannot be read\n");
}

Test(Builtin, echo_valid_0, .init = redirect_all_std)
{
	t_shell *shell;

	shell = create_minishell(environ);
	shell->cmd_line = "echo hello";
	parse_and_exec_cmd(shell, shell->cmd_line);
	cr_assert_stdout_eq_str("hello\n");
	free_minishell(shell);
}

Test(Builtin, echo_valid_1, .init = redirect_all_std)
{
	t_shell *shell;

	shell = create_minishell(environ);
	shell->cmd_line = "echo 'hello and goodbye'";
	parse_and_exec_cmd(shell, shell->cmd_line);
	cr_assert_stdout_eq_str("hello and goodbye\n");
	free_minishell(shell);
}

Test(Builtin, echo_option_0, .init = redirect_all_std)
{
	t_shell *shell;

	shell = create_minishell(environ);
	shell->cmd_line = "echo -n hello";
	parse_and_exec_cmd(shell, shell->cmd_line);
	cr_assert_stdout_eq_str("hello");
	free_minishell(shell);
}












// Test(Execution, single_cmd, .init = redirect_all_stdout)
// {
// 	t_shell	*shell;

// 	shell = create_minishell(environ);
// 	shell->cmd_line = "echo hello";
// 	parse_and_exec_cmd(shell, shell->cmd_line);
// 	fprintf(stderr, "%s", ((t_cmd *)shell->cmd_list->content)->cmd_path);
// 	cr_assert_stderr_eq_str("echo");
// 	free_minishell(shell);
// }
