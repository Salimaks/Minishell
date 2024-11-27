/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 12:02:49 by skassimi          #+#    #+#             */
/*   Updated: 2024/11/27 16:30:09 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

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
	s_cmd	*next;
}	t_cmd;

typedef struct	s_cmd_table
{
	t_cmd	*first_cmd;	// start of the linked list of commands
	int		cmd_count;	// total of commands in commmand line
	int		curr_cmd;	// command currently being executed
	int		*pipefd[2];	// array of pipe fd [2] required by each pipe()
	char	**path;		// extracted PATH variable of the env
	char	**env;		// env received at start of program
}	t_cmd_tab;

typedef struct s_maman
{
	char		**line;
	t_cmd_tab	cmd_tab;	// struct organizing and keeping count of cmds
	char		**env;		// env received at start of program
}	t_maman;

#endif
