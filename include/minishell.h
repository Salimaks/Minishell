/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skassimi <skassimi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 12:02:49 by skassimi          #+#    #+#             */
/*   Updated: 2024/11/27 12:04:16 by skassimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

typedef struct s_cmd
{
	char	**argv;
	int		argc;
	char	*outfile;
	char	*infile;
	s_cmd	*next;
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
}    t_maman

#endif