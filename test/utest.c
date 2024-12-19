/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utest.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 15:58:43 by mkling            #+#    #+#             */
/*   Updated: 2024/12/19 16:08:48 by mkling           ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "minishell.h"
#include <criterion/criterion.h>
#include <criterion/redirect.h>

extern char	**environ;

void	redirect_all_std(void)
{
	cr_redirect_stderr();
	cr_redirect_stdout();
}

/* PARSING: */

/* Lexer */




/* Syntax */
Test(Syntax, Valid_Single_Command)
{
	t_shell	*shell;

	shell = create_minishell(environ);
	shell->cmd_line = "echo hello";
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

Test(Syntax, Invalid_Pipe, .init=redirect_all_std)
{
	t_shell	*shell;

	shell = create_minishell(environ);
	shell->cmd_line = "   | echo hello";
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



/* EXECUTION: */

/* Command Path*/
Test(Execution, get_cmd_path)
{
	t_shell shell;
	t_cmd cmd;
	char *argv[] = {"ls", NULL};

	shell.env = environ;
	extract_paths(&shell);
	cmd.argv = argv;
	get_cmd_path(&shell, &cmd);
	cr_assert(cmd.cmd_path != NULL);
}

Test(Execution, open_file)
{
	t_cmd	cmd;
	t_file	file;

	file.path = "testfile";
	open_file(&file, &cmd, WRITE);
	cr_assert(file.fd >= 0);
	close(file.fd);
	unlink(file.path);
}

Test(Execution, get_infile_fd)
{
	t_shell shell;
	t_cmd cmd;
	t_file file;
	t_list node;

	shell.critical_er = 0;
	file.path = "testfile";
	file.mode = INFILE;
	node.content = &file;
	node.next = NULL;
	cmd.infiles = &node;

	open_file(&file, &cmd, WRITE);
	close(file.fd);

	int fd = get_infile_fd(&shell, &cmd);
	cr_assert(fd >= 0);
	close(fd);
	unlink(file.path);
}

Test(Execution, get_outfile_fd)
{
	t_cmd cmd;
	t_file file;
	t_list node;

	file.path = "testfile";
	file.mode = OUTFILE;
	node.content = &file;
	node.next = NULL;
	cmd.outfiles = &node;

	int fd = get_outfile_fd(&cmd);
	cr_assert(fd >= 0);
	close(fd);
	unlink(file.path);
}

Test(Execution, Echo_Command, .init = redirect_all_std)
{
	t_shell *shell;

	shell = create_minishell(environ);
	shell->cmd_line = "echo hello";
	parse_and_exec_cmd(shell, shell->cmd_line);
	cr_assert_stdout_eq_str("hello\n");
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
