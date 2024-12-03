/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skassimi <skassimi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 12:02:49 by skassimi          #+#    #+#             */
/*   Updated: 2024/11/29 11:01:47 by skassimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <readline/readline.h>

typedef struct s_cmd
{
	char	**argv;
	int		argc;
	char	*outfile;
	char	*infile;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_command_table
{
    t_cmd *first;
    int cmd_count;
}    t_command_table;

typedef struct s_maman
{
    t_command_table *cmd;
    char **line;
    char **env;
}    t_maman;

//signals
void signals(void);
#endif