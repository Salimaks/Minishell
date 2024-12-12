/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 15:42:30 by mkling            #+#    #+#             */
/*   Updated: 2024/12/12 12:54:25 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*generate_heredoc_filepath(t_cmd_tab *cmd_tab)
{
	char	temp[249];
	char	*heredoc_path;
	int		index;

	heredoc_path = HEREDOC_LOC;
	while (1)
	{
		if (access(heredoc_path, F_OK) != 0)
			break ;
		heredoc_path = ft_strjoinfree(heredoc_path, temp);
		if (!heredoc_path)
			return (set_error(MALLOC_FAIL, cmd_tab, "Heredoc"), NULL);
		index = 249;
		while (index >= 0)
		{
			if (temp[index] == 'z')
				temp[index--] = 'a';
			else
			{
				temp[index]++;
				break ;
			}
		}
	}
	return (heredoc_path);
}

void	assemble_heredoc(t_cmd_tab *cmd_tab, t_cmd *cmd, t_list *file_node)
{
	t_file	*file;
	char	*result;

	file = (t_file *)file_node->content;
	if (file->mode != HEREDOC)
		return ;
	open_file(file, cmd, WRITE);
	if (file->fd < 0)
		return (set_error(READ_ERROR, cmd_tab, "Unable to create heredoc"));
	while (1)
	{
		write(STDIN_FILENO, "heredoc> ", 9);
		result = get_next_line(STDIN_FILENO);
		if (ft_strncmp(file->delimiter, result, ft_strlen(file->delimiter)) == 0)
			break ;
		write(file->fd, result, ft_strlen(result));
	}
	close(file->fd);
}

void	destroy_heredoc(t_cmd_tab *cmd_tab, t_list *file_node)
{
	t_file	*file;

	if (catch_error(cmd_tab))
		return ;
	file = (t_file *)(file_node)->content;
	if (file->mode != HEREDOC)
		return ;
	unlink(file->path);
}
