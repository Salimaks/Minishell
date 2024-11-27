/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 12:02:49 by skassimi          #+#    #+#             */
/*   Updated: 2024/11/27 18:52:48 by mkling           ###   ########.fr       */
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
}	t_cmd;

typedef struct	s_cmd_table
{
	t_cmd	**cmd_array;// array of commands mini sructures
	int		cmd_count;	// total of commands in commmand line
	int		index;		// index of command currently being executed
	int		**pipefd;	// array of pipe fd [2] required by each pipe()
	char	**path;		// extracted PATH variable of the env
	char	**env;		// env received at start of program
}	t_cmd_tab;

typedef struct s_maman
{
	char		**line;
	t_cmd_tab	cmd_tab;	// struct organizing and keeping count of cmds
	char		**env;		// env received at start of program
}	t_maman;




/* EXECUTION */

void	fork_exit_if(int condition, int error_code, t_cmd *cmd,
			char *error_message);


/* REDIRECTION */

void	connect_pipe(t_cmd_tab *cmd_tab);


/* READABILITY */

int		is_last_cmd(t_cmd_tab *cmd_tab);
int		is_first_cmd(t_cmd_tab *cmd_tab);
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
