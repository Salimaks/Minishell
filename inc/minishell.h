/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 12:02:49 by skassimi          #+#    #+#             */
/*   Updated: 2024/12/04 08:36:37 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <string.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft/inc/libft.h"

typedef struct s_token
{
	int				lexem;
	char			*content;
	char			letter;
	struct s_token	*next;
	struct s_token	*prev;
}	t_token;

typedef struct s_cmd
{
	char			**argv;		// array created with cmd_path, then arguments
	char			*outfile;	// filepath from which input must be redirected
	char			*infile;	// filepath to which output must be redirected
	char			*cmd_path;	// binary filepath, absolute/through PATH
	int				exit_code;	// value returned by the execution of command
	size_t			cmd_index;	// number of the command among other commands
	int				fork_pid;	// process id of fork sent to execute command
	int				pipe_fd[2];	// array of 2 pipe fd required for the pipe()
	struct s_cmd	*next;
	struct s_cmd	*prev;
}	t_cmd;

typedef struct s_cmd_table
{
	char		*cmd_line;		// readline return
	t_token		*token_list;	// linked list of tokens id from cmd line
	t_cmd		*cmd_list;		// first commands structures in linked list
	size_t		cmd_count;		// total of commands in commmand line
	size_t		index;			// index of command currently being executed
	char		**env;			// env received at start of program
	char		**paths;		// extracted PATH variable of the env
	int			stdin_fd;
	int			critical_er;	// flag if critical error in parent process
}	t_cmd_tab;


/* INPUT */

void		extract_paths(t_cmd_tab *cmd_tab);
void		init_readline(char **env);
void		signals(void);

/* PARSING */

void		scan(t_cmd_tab *cmd_tab);
void		add_token(t_cmd_tab *cmd_tab, int type, char letter);
void		pop_token(t_token *token);
void		merge_token(t_cmd_tab *cmd_tab, t_token *start, t_token *end);
t_token		*find_token_in_list(t_token *start, int letter);
void		lexer(t_cmd_tab *cmd_tab);
t_cmd_tab	*create_cmd_tab(char **env);
t_cmd		*create_cmd(t_cmd_tab *cmd_tab);
void		append_cmd(t_cmd *cmd, t_cmd_tab *cmd_tab);
void		load_cmd(t_cmd_tab *cmd_tab, t_token *token);
void		parse(t_cmd_tab *cmd_tab, t_token *start);

/* EXECUTION */

void		create_fork(t_cmd_tab *cmd_tab);
void		get_cmd_path(t_cmd *cmd, t_cmd_tab *cmd_tab);
void		fork_exit_if(int condition, int error_code, t_cmd *cmd,
				char *error_message);
int			execute_all_cmd(t_cmd_tab *cmd_tab);

/* REDIRECTION */

void		open_pipes(t_cmd_tab *cmd_tab);
void		connect_pipe(t_cmd_tab *cmd_tab);
void		close_pipes(t_cmd_tab *cmd_tab);

/* READABILITY */

int			is_last_cmd(t_cmd_tab *cmd_tab);
int			is_first_cmd(t_cmd_tab *cmd_tab);
t_cmd		*get_current_cmd(t_cmd_tab *cmd_tab);
t_cmd		*get_last_cmd(t_cmd_tab *cmd_tab);
int			get_last_cmd_exit_code(t_cmd_tab *cmd_tab);

/* ERROR HANDLING */

void		set_error_if(int condition, int err_code, t_cmd_tab *cmd_tab,
				char *message);
void		set_error(int err_code, t_cmd_tab *cmd_tab, char *err_message);
int			catch_error(t_cmd_tab *cmd_tab);

/* CLEAN UP */

void		free_token(t_token *token);
void		free_cmd_tab(t_cmd_tab *cmd_tab);
void		free_token_list(t_cmd_tab *cmd_tab);
void		free_cmd_list(t_cmd_tab *cmd_tab);

# define TRUE		1
# define FALSE		0
# define DELIMITERS	"'\"()"
# define OPERATORS	"|><$"
# define WHITESPACES " \n\t\0"

enum e_lexem
{
	NONE		= 0,
	WORD		= 1,
	WHITESPACE	= 2,
	DELIMITER	= 3,
	OPERATOR	= 4,
	END			= 5,
	CMD,
	VARIABLE,
	STRING,
	DOUB_QUOTE	= '"',
	SING_QUOTE	= '\'',
	PIPE		= '|',
	GREATER		= '>',
	LESSER		= '<',
	HEREDOC,
	END_OF_HERED,
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
	CANT_EXECUTE_CMD = 126,
	CANT_FIND_CMD = 127,
};

enum e_pipe_fd
{
	READ = 0,
	WRITE = 1,
	APPEND = 2,
};


#endif
