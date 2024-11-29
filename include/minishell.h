/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 12:02:49 by skassimi          #+#    #+#             */
/*   Updated: 2024/11/29 13:24:43 by alex             ###   ########.fr       */
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
# include "libft/inc/libft.h"

typedef struct s_cmd
{
	char			**argv;		// array created with cmd_path, then arguments
	char			**env;		// needed by executing fork for execve()
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
	t_cmd	*cmd_list;		// first commands mini structures in linked list
	int		cmd_count;		// total of commands in commmand line
	int		index;			// index of command currently being executed
	char	**env;			// env received at start of program
	char	**path;			// extracted PATH variable of the env
	int		critical_er;	// flag if critical error in parent process
}	t_cmd_tab;

typedef struct s_maman
{
	char		**line;
	t_cmd_tab	cmd_tab;	// struct organizing and keeping count of cmds
	char		**env;		// env received at start of program
}	t_maman;



/* EXECUTION */

t_cmd_tab	*create_cmd_tab(char **argv, char **env);
void		create_pipe(t_cmd_tab *cmd_tab);
void		create_fork(t_cmd_tab *cmd_tab);
void		close_pipe(t_cmd_tab *cmd_tab);
void		fork_exit_if(int condition, int error_code, t_cmd *cmd,
				char *error_message);

/* REDIRECTION */

void		connect_pipe(t_cmd_tab *cmd_tab);
int			open_file(char *filepath, int mode);

/* READABILITY */

int			is_last_cmd(t_cmd_tab *cmd_tab);
int			is_first_cmd(t_cmd_tab *cmd_tab);
t_cmd		*get_current_cmd(t_cmd_tab *cmd_tab);
t_cmd		*get_last_cmd(t_cmd_tab *cmd_tab);
int			are_error(t_cmd_tab *cmd_tab);
int			open_file(char *filepath, int mode);
int			get_last_cmd_exit_code(t_cmd_tab *cmd_tab);

# define TRUE	1
# define FALSE	0

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
