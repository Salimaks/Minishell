/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skassimi <skassimi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 12:02:49 by skassimi          #+#    #+#             */
/*   Updated: 2025/01/09 17:32:55 by skassimi         ###   ########.fr       */
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
# include "/home/skassimi/Minishell/inc/libft/inc/libft.h"

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
	t_list			*arg_list;	// linked list of command arguments
	t_list			*infiles;	// linked list of input files
	t_list			*outfiles;	// linked list of output files
	int				append;
	int				fd_out;		// fd of the final output redirection
	int				fd_in;		// fd of the final input redirection
	int				exit_code;	// value returned by the execution of command
	size_t			cmd_index;	// number of the command among other commands
	int				fork_pid;	// process id of fork sent to execute command
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_ast
{
	int				type;		// either pipe, or, and or single cmd node
	void			*content;	// may contain pointer to cmd structure
	struct s_ast	*left;		// part of the cmd line left of the operator
	struct s_ast	*right;		// part of the cmd line right of the operator
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
	int			std_out;		// preserved stdout
	int			std_in;			// preserved stdin
	int			critical_er;	// flag if critical error in parent process
	int			last_exit_code;	// result of the last command
}	t_shell;

/* SIGNAL */

void		handle_siquit(int sig);
void		signals(void);

/* INPUT */

void		parse_and_exec_cmd(t_shell *shell, char *input);
void		extract_env_as_linked_list(t_shell *shell);
void		extract_paths(t_shell *shell);
void		init_readline(t_shell *shell);

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
t_list		*tokenizer(t_shell *shell, char *input);
void		free_tokens(t_list *current);
char		*ft_strndup(const char *src, int size);
int			is_not_space(char c);
void	tokenize_special(char **input, t_list **tokens);


/* HEREDOC */

char		*generate_heredoc_filepath(t_shell *shell);
void		assemble_heredoc(t_shell *shell, t_cmd *cmd, t_list *file_node);
void		destroy_heredoc(t_shell *shell, t_list *file_node);

/* EXECUTION */

int			exec_tree(t_shell *shell, t_tree *tree, bool piped);
int			exec_pipe(t_shell *shell, t_tree *tree);
int			exec_single_cmd(t_shell *shell, t_tree *tree, bool piped);
int			create_fork(t_shell *shell, int	*fork_pid);
void		get_cmd_path(t_shell *shell, t_cmd *cmd);
void		put_arg_in_array(t_cmd *cmd);

/* BUILT IN */

int			cd(t_shell *shell, char *path);
int			echo(char **argv, int fdout);
int			env(t_shell *shell, int fdout);
int			export(t_shell *shell, char **argv, int fdout);
int			unset(t_shell *shell, char **argv);
int			pwd(int fdout);
int			exit_shell(t_shell *shell, char **argv);
int			exec_builtin(t_shell *shell, t_cmd *cmd);
int			is_builtin(t_cmd *cmd);

/* REDIRECTION */

void		open_file(t_file *file, t_cmd *cmd, int mode);
void		redirect_for_cmd(t_shell *shell, t_cmd *cmd);
int			create_pipe(t_shell *shell, int *pipe_fd);
int			connect_pipes_and_exec(t_shell *shell, t_tree *tree,
				int pipe_fd[2], int mode);
void		close_pipe(int *pipe_fd);
void		set_infile_fd(t_shell *shell, t_cmd *cmd);
void		set_outfile_fd(t_cmd *cmd);
void		reset_std(t_shell *shell, bool piped);

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
	ENV			= 7,
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
	SUCCESS = 0,
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
