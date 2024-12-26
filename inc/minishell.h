/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 12:02:49 by skassimi          #+#    #+#             */
/*   Updated: 2024/12/26 12:40:27 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <string.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdlib.h>
# include <stdbool.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft/inc/libft.h"

typedef struct s_token
{
	int				lexem;		// type as determined by the lexer
	char			letter;		// letter or first letter
	char			*content;	// malloced words or letter
}	t_token;

typedef struct s_files
{
	int				mode;		// infile, outfile, heredoc or append
	char			*path;		// pathfile
	char			*delim;		// delimiter if heredoc, else NULL
	int				fd;			// resulting fd once opened
	bool			is_quoted;	// is a quote in delimiter, meaining expand
}	t_file;

typedef struct s_cmd
{
	char			**argv;		// array created with cmd_path, then arguments
	char			*cmd_path;	// binary filepath, absolute/through PATH
	t_list			*arg_list;
	t_list			*redirect;
	t_list			*infiles;	// linked list of input files
	t_list			*outfiles;	// linked list of output files
	int				fd_out;
	int				fd_in;
	int				exit_code;	// value returned by the execution of command
	size_t			cmd_index;	// number of the command among other commands
	int				fork_pid;	// process id of fork sent to execute command
	int				pipe_fd[2];	// array of 2 pipe fd required for the pipe()
}	t_cmd;

typedef struct s_ast
{
	int				type;
	void			*content;
	struct s_ast	*left;
	struct s_ast	*right;
}	t_tree;

typedef struct s_shell
{
	char		*cmd_line;		// readline return
	t_list		*token_list;	// linked list of tokens id from cmd line
	t_list		*cmd_list;		// linked list of commands as a structs
	t_list		*env_list;		// linked list of env strings
	t_tree		*tree_root;		// abstract syntaxic tree
	int			pipe_fd[2][2];	// two pair of pipe fd for all pipe execution
	size_t		cmd_count;		// total of commands in commmand line
	size_t		index;			// index of command currently being executed
	char		**env;			// env received at start of program
	char		**paths;		// extracted PATH variable of the env
	int			std_out;
	int			std_in;
	int			critical_er;	// flag if critical error in parent process
}	t_shell;

/* SIGNAL */

void		handle_siquit(int sig);

/* INPUT */

void		parse_and_exec_cmd(t_shell *shell, char *input);
void		extract_env_as_linked_list(t_shell *shell);
void		extract_paths(t_shell *shell);
void		init_readline(t_shell *shell);
void		signals(void);

/* PARSING */

t_shell		*create_minishell(char **env);
void		scan(t_shell *shell);
void		add_token(t_shell *shell, int type, char letter);
void		merge_token(t_shell *shell, t_list *start);
void		lexer(t_shell *shell);
int			check_syntax(t_shell *shell, t_list *token_list);
t_tree		*create_branch(t_shell *shell, int type, void *content);
t_cmd		*create_cmd(void);
void		create_file(t_shell *shell, t_cmd *cmd, t_token *token);
void		apply_to_list(t_shell *s, t_list *n, void f(t_shell *, t_list *));
void		parser(t_shell *shell);

/* HEREDOC */

char		*generate_heredoc_filepath(t_shell *shell);
void		assemble_heredoc(t_shell *shell, t_cmd *cmd, t_list *file_node);
void		destroy_heredoc(t_shell *shell, t_list *file_node);

/* EXECUTION */

int			exec_tree(t_shell *shell, t_tree *tree, bool piped);
int			execute_all_cmd(t_shell *shell);
void		open_pipe(t_shell *shell, t_list *node);
void		create_fork(t_shell *shell, int	*fork_pid);
void		get_cmd_path(t_shell *shell, t_cmd *cmd);
void		put_arg_in_array(t_cmd *cmd);

/* BUILT IN */

int			cd(t_shell *shell, char *path);
int			echo(char **argv, int fdout);
int			env(t_shell *shell, int fdout);
int			export(t_shell *shell, char **argv, int fdout);
int			unset(t_shell *shell, char **argv);
int			pwd(int fdout);
int			exit_shell(t_shell *shell);
int			exec_builtin(t_shell *shell, t_cmd *cmd);
int			is_builtin(t_cmd *cmd);

/* REDIRECTION */

void		open_file(t_file *file, t_cmd *cmd, int mode);
void		redirect_for_cmd(t_shell *shell, t_cmd *cmd);
void		close_pipe(t_shell *shell, t_list *node);
void		set_infile_fd(t_shell *shell, t_cmd *cmd);
void		set_outfile_fd(t_cmd *cmd);
void		redirect_io(t_shell *shell, t_cmd *cmd, int input, int output);

/* READABILITY */

int			is_last_cmd(t_list *node);
int			is_first_cmd(t_list *node);

/* ERROR HANDLING */

void		set_error_if(int cond, int err_code, t_shell *shell, char *message);
void		set_error(int err_code, t_shell *shell, char *err_message);
void		set_cmd_error(int err_code, t_cmd *cmd, char *err_message);
int			catch_error(t_shell *shell);
void		print_error(void);

/* CLEAN UP */

void		free_token(void *token);
void		free_cmd(void *cmd);
void		free_file(void *file);
void		free_tree(t_tree **tree);
void		free_minishell(t_shell *shell);

/* DEBUG */

void		print_tokens(t_list *first);

# define TRUE			1
# define FALSE			0
# define DELIMITERS		"'\"()"
# define OPERATORS		"|><$"
# define BLANKS	" \n\t"
# define HEREDOC_LOC	"tmp/heredoc"
# define SHELL_NAME		"shell"

enum e_lexem
{
	NONE		= 0,
	WORD		= 1,
	BLANK		= 2,
	DELIMITER	= 3,
	OPERATOR	= 4,
	END			= 5,
	START		= 6,
	VARIABLE,
	CMD,
	OUTFILE,
	INFILE,
	HEREDOC,
	APPEND,
	STRING,
	DOUB_QUOTE	= '"',
	SING_QUOTE	= '\'',
	PIPE		= '|',
	GREATER		= '>',
	LESSER		= '<',
};

enum e_exit_code
{
	FAIL = -1,
	INVALID_FD = -1,
	OK = 0,
	MALLOC_FAIL = 1,
	GENERAL_ERROR = 1,
	PIPE_ERROR = 1,
	FORK_ERROR = 1,
	DUP_ERROR = 1,
	NO_FILE = 1,
	READ_ERROR = 1,
	OPEN_ERROR = 1,
	SYNTAX_ERROR = 1,
	TOO_MANY_HEREDOC = 1,
	CANT_EXECUTE_CMD = 126,
	CANT_FIND_CMD = 127,
};

enum e_pipe_fd
{
	READ = 0,
	WRITE = 1,
};

enum e_ast
{
	AST_PIPE,
	AST_CMD,
	AST_REDIRECTION,
	AST_AND,
	AST_OR,
	AST_SUB,
};

enum e_tree_mode
{
	NO_PIPE,
	IS_PIPE,
	AST_LEFT,
	AST_RIGHT,
};

#endif
