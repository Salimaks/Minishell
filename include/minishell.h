/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 12:02:49 by skassimi          #+#    #+#             */
/*   Updated: 2024/11/30 00:54:49 by alex             ###   ########.fr       */
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
# include <readline/readline.h>
# include <readline/history.h>
# include "libft/inc/libft.h"

typedef struct s_lexer
{
	int				type;
	int				subtype;
	char			*content;
	struct s_lexer	*next;
	struct s_lexer	*prev;
}	t_token;

typedef struct s_cmd
{
	char			**argv;		// array created with cmd_path, then arguments
	char			*outfile;	// filepath from which input must be redirected
	char			*infile;	// filepath to which output must be redirected
	char			*cmd_path;	// binary filepath, absolute/through PATH
	int				exit_code;	// value returned by the execution of command
	int				cmd_index;	// number of the command among other commands
	int				fork_pid;	// process id of fork sent to execute command
	int				pipe_fd[2];	// array of 2 pipe fd required for the pipe()
	struct s_cmd	*next;
	struct s_cmd	*prev;
}	t_cmd;

typedef struct s_cmd_table
{
	char	*cmd_line;		// readline return
	t_token	*token_list;	// linked list of tokens identified from cmd line
	t_cmd	*cmd_list;		// first commands mini structures in linked list
	int		cmd_count;		// total of commands in commmand line
	int		index;			// index of command currently being executed
	char	**env;			// env received at start of program
	char	**paths;		// extracted PATH variable of the env
	int		critical_er;	// flag if critical error in parent process
}	t_cmd_tab;

typedef struct s_maman
{
	char		**line;
	t_cmd_tab	cmd_tab;	// struct organizing and keeping count of cmds
	char		**env;		// env received at start of program
}	t_maman;


/* PARSING */

void		extract_paths(t_cmd_tab *cmd_tab);
void		init_readline(t_cmd_tab *cmd_tab);
void		id_delimiter(t_cmd_tab *cmd_tab);
void		id_operator(t_cmd_tab *cmd_tab);
void		id_whitespace(t_cmd_tab *cmd_tab);
void		id_word(t_cmd_tab *cmd_tab);
void		add_token(t_cmd_tab *cmd_tab, int type, char *content);
void		tokenize(t_cmd_tab *cmd_tab);
void		free_token_list(t_cmd_tab *cmd_tab);

/* EXECUTION */

t_cmd_tab	*create_cmd_tab(char **env);
void		create_pipe(t_cmd_tab *cmd_tab);
void		create_fork(t_cmd_tab *cmd_tab);
void		get_cmd_path(t_cmd *cmd, t_cmd_tab *cmd_tab);
void		close_pipe(t_cmd_tab *cmd_tab);
void		fork_exit_if(int condition, int error_code, t_cmd *cmd,
				char *error_message);
int			execute_all_cmd(t_cmd_tab *cmd_tab);

/* REDIRECTION */

void		connect_pipe(t_cmd_tab *cmd_tab);
int			open_file(char *filepath, int mode);

/* READABILITY */

int			is_last_cmd(t_cmd_tab *cmd_tab);
int			is_first_cmd(t_cmd_tab *cmd_tab);
t_cmd		*get_current_cmd(t_cmd_tab *cmd_tab);
t_cmd		*get_last_cmd(t_cmd_tab *cmd_tab);
int			catch_error(t_cmd_tab *cmd_tab);
int			open_file(char *filepath, int mode);
int			get_last_cmd_exit_code(t_cmd_tab *cmd_tab);

/* ERROR HANDLING */

void		set_error_if(int condition, int err_code, t_cmd_tab *cmd_tab,
				char *err_message);
int			catch_error(t_cmd_tab *cmd_tab);

/* CLEAN UP */

void		free_cmd_tab(t_cmd_tab *cmd_tab);

# define TRUE		1
# define FALSE		0
# define DELIMITERS	"\'\""
# define OPERATORS	"|><"
# define WHITESPACE	" \n\t"

enum e_token_type
{
	NONE,
	WORD,
	WHITE_SPACE,
	DELIMITER,
	STRING,
	OPERATOR,
	END,
};

enum e_token_subtype
{
	CMD,
	SING_QUOTE,
	DOUB_QUOTE,
	PIPE,
	INFILE,
	OUTFILE,
	HEREDOC,
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
