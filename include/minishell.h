/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 12:02:49 by skassimi          #+#    #+#             */
/*   Updated: 2024/11/28 12:52:27 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <string.h>
# include <unistd.h>
# include <fcntl.h>

typedef struct s_cmd
{
	int		argc;
	char	**argv;		// array created with cmd_path, then all arguments
	char	**env;		// needed by executing fork for execve()
	char	*outfile;	// filepath from which input must be redirected
	char	*infile;	// filepath to which output must be redirected
	char	*cmd_path;	// binary filepath, absolute or through env's PATH
	int		exit_code;	// value returned by the execution of the command
	int		cmd_index;	// number of the command among the other commands
	int		fork_pid;	// process id of the fork sent to execute command
	int		pipe_fd[2];	// array of 2 pipe fd required for the pipe()
}	t_cmd;

typedef struct	s_cmd_table
{
	t_cmd	**cmd_array;// array of commands mini structures
	int		cmd_count;	// total of commands in commmand line
	int		index;		// index of command currently being executed
	char	**env;		// env received at start of program
	char	**path;		// extracted PATH variable of the env
	int		critical_er	// flag if critical error in parent process
}	t_cmd_tab;

typedef struct s_maman
{
	char		**line;
	t_cmd_tab	cmd_tab;	// struct organizing and keeping count of cmds
	char		**env;		// env received at start of program
}	t_maman;




/* EXECUTION */

void	create_pipe(t_cmd_tab *cmd_tab);
void	create_fork(t_cmd_tab *cmd_tab);
void	close_pipe(t_cmd_tab *cmd_tab);
void	fork_exit_if(int condition, int error_code, t_cmd *cmd,
			char *error_message);

/* REDIRECTION */

void	connect_pipe(t_cmd_tab *cmd_tab);
int		open_file(char *filepath, int mode);

/* READABILITY */

int		is_last_cmd(t_cmd_tab *cmd_tab);
int		is_first_cmd(t_cmd_tab *cmd_tab);
t_cmd	*get_current_cmd(t_cmd_tab *cmd_tab);
int		are_error(t_cmd_tab *cmd_tab);
int		open_file(char *filepath, int mode);

# define TRUE	1
# define FALSE	0

enum e_exit_code {
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

enum e_pipe_fd {
	READ = 0,
	WRITE = 1,
	APPEND = 2,
};


#endif
