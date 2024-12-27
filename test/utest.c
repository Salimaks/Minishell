/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utest.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 15:58:43 by mkling            #+#    #+#             */
/*   Updated: 2024/12/26 21:58:55 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <criterion/new/assert.h>

extern char	**environ;

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

	shell.env = environ;
	extract_paths(&shell);
	cmd.arg_list = ft_lstnew("ls");
	get_cmd_path(&shell, &cmd);
	cr_assert(cmd.cmd_path != NULL);
	cr_assert(eq(str, cmd.cmd_path, "/usr/bin/ls"));
}

Test(Execution, set_infile_forbidden, .init=redirect_all_std)
{
	t_shell	shell;
	t_cmd	cmd;
	t_file	file;
	t_list	node;

	shell.critical_er = 0;
	file.path = "test/forbidden.txt";
	file.mode = INFILE;
	node.content = &file;
	node.next = NULL;
	node.prev = NULL;
	cmd.infiles = &node;
	cmd.arg_list = ft_lstnew("echo");
	cmd.fd_in = -1;

	set_infile_fd(&shell, &cmd);
	cr_assert(eq(int, cmd.fd_in, -1));
	cr_assert_stderr_eq_str("shell: echo: Forbidden input file\n");
}

Test(Execution, set_outfile_forbidden, .init=redirect_all_std)
{
	t_cmd	cmd;
	t_file	file;
	t_list	node;

	file.path = "test/forbidden.txt";
	file.mode = OUTFILE;
	node.content = &file;
	node.next = NULL;
	node.prev = NULL;
	cmd.outfiles = &node;
	cmd.arg_list = ft_lstnew("echo");
	cmd.fd_out = -1;

	set_outfile_fd(&cmd);
	cr_assert(eq(int, cmd.fd_out, -1));
	cr_assert_stderr_eq_str("shell: echo: Forbidden output file\n");
}

Test(Execution, no_path_relative_command, .init=redirect_all_std)
{
	t_cmd	cmd;
	t_shell	shell;

	cmd.arg_list = ft_lstnew("ls");
	shell.paths = NULL;
	get_cmd_path(&shell, &cmd);
	cr_assert_stderr_eq_str("shell: ls: No PATH variable found\n");
}

Test(Execution, unknown_command, .init=redirect_all_std)
{
	t_cmd	cmd;
	t_shell	shell;
	char	*paths[] = {"usr/bin", NULL};

	cmd.arg_list = ft_lstnew("eccho");
	shell.paths = paths;
	get_cmd_path(&shell, &cmd);
	cr_assert_stderr_eq_str("shell: eccho: Command not found\n");
}

Test(Builtin, echo_valid_0, .init=redirect_all_std)
{
	char	*argv[] = {"echo", "hello", NULL};
	int		exit_code;

	exit_code = echo(argv, STDOUT_FILENO);
	cr_assert(eq(int, exit_code, 0));
	cr_assert_stdout_eq_str("hello\n");
}

Test(Builtin, echo_valid_1, .init=redirect_all_std)
{
	char	*argv[] = {"echo", "hello and goodbye", NULL};
	int		exit_code;

	exit_code = echo(argv, STDOUT_FILENO);
	cr_assert(eq(int, exit_code, 0));
	cr_assert_stdout_eq_str("hello and goodbye\n");
}

Test(Builtin, echo_option_0, .init=redirect_all_std)
{
	char	*argv[] = {"echo", "-n", "hello", NULL};
	int		exit_code;

	exit_code = echo(argv, STDOUT_FILENO);
	cr_assert(eq(int, exit_code, 0));
	cr_assert_stdout_eq_str("hello");
}

Test(Builtin, echo_option_1, .init=redirect_all_std)
{
	char	*argv[] = {"echo", "-n", "hello and goodbye", NULL};
	int		exit_code;

	exit_code = echo(argv, STDOUT_FILENO);
	cr_assert(eq(int, exit_code, 0));
	cr_assert_stdout_eq_str("hello and goodbye");
}

Test(Complete, pipe_basic_0, .init=redirect_all_std)
{
	t_shell *shell;

	shell = create_minishell(environ);
	shell->cmd_line = "echo hello | cat -e";
	parse_and_exec_cmd(shell, shell->cmd_line);
	cr_assert(eq(int, shell->last_exit_code, 0));
	cr_assert_stdout_eq_str("hello$\n");
	free_minishell(shell);
}

Test(Complete, pipe_synchro_0, .init=redirect_all_std)
{
	t_shell *shell;

	shell = create_minishell(environ);
	shell->cmd_line = "yes | head -n 2";
	parse_and_exec_cmd(shell, shell->cmd_line);
	cr_assert(eq(int, shell->last_exit_code, 0));
	cr_assert_stdout_eq_str("y\ny\n");
	free_minishell(shell);
}
