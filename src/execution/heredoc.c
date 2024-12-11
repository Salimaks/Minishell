/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 15:42:30 by mkling            #+#    #+#             */
/*   Updated: 2024/12/11 22:43:25 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*generate_heredoc_filepath(t_cmd_tab *cmd_tab)
{
	char	*heredoc_filepath;

	heredoc_filepath = ft_strjoin(HEREDOC_LOC, "heredoc");
	if (!heredoc_filepath)
		return (set_error(MALLOC_FAIL, cmd_tab,
				"Failed to malloc heredoc"), NULL);
	while (access(heredoc_filepath, F_OK))
	{
		heredoc_filepath = ft_strjoinfree(heredoc_filepath, "a");
		if (!heredoc_filepath)
			return (set_error(MALLOC_FAIL, cmd_tab,
					"Failed to malloc heredoc"), NULL);
	}
	return (heredoc_filepath);
}

void	assemble_heredoc(t_cmd_tab *cmd_tab, char *heredoc_path, char *limiter)
{
	int		heredoc_fd;
	char	*result;

	heredoc_fd = open_file(heredoc_path, WRITE);
	if (heredoc_fd < 0)
		return (set_error(READ_ERROR, cmd_tab, "Unable to create heredoc"));
	while (1)
	{
		write(STDIN_FILENO, "heredoc> ", 9);
		result = get_next_line(STDIN_FILENO);
		if (ft_strncmp(limiter, result, ft_strlen(limiter)) == 0)
			break ;
		write(heredoc_fd, result, ft_strlen(result));
	}
	close(heredoc_fd);
}
