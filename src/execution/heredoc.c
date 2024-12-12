/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 15:42:30 by mkling            #+#    #+#             */
/*   Updated: 2024/12/12 19:52:49 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*increment_char(t_cmd_tab *cmd_tab, char *string)
{
	size_t	len;
	char	letter;

	if (!string)
		return (NULL);
	letter = 'a';
	len = ft_strlen(string);
	if (len > ft_strlen(HEREDOC_LOC)
		|| string[len - 1] != 'z' || string[len - 1] != 'Z')
	{
		string[len - 1] = string[len - 1]++;
		return (string);
	}
	else if (len < 256)
		return (ft_strjoinfree(string, &letter));
	return (set_error(TOO_MANY_HEREDOC, cmd_tab,
			"reached end of posible random name"), NULL);
}

char	*generate_heredoc_filepath(t_cmd_tab *cmd_tab)
{
	char	*heredoc_path;

	heredoc_path = ft_strdup(HEREDOC_LOC);
	while (1)
	{
		if (access(heredoc_path, F_OK) != 0)
			break ;
		heredoc_path = increment_char(cmd_tab, heredoc_path);
		if (!heredoc_path)
			return (set_error(MALLOC_FAIL, cmd_tab, "Heredoc"), NULL);
	}
	return (heredoc_path);
}

void	assemble_heredoc(t_cmd_tab *cmd_tab, t_cmd *cmd, t_list *file_node)
{
	t_file	*file;
	char	*result;
	char	*heredoc_index;

	file = (t_file *)file_node->content;
	if (file->mode != HEREDOC)
		return ;
	open_file(file, cmd, WRITE);
	if (file->fd < 0)
		return (set_error(READ_ERROR, cmd_tab, "Unable to create heredoc"));
	heredoc_index = ft_strjoinfree(ft_itoa(cmd->cmd_index + 1), " heredoc> ");
	while (1)
	{
		write(STDIN_FILENO, heredoc_index, ft_strlen(heredoc_index));
		result = get_next_line(STDIN_FILENO);
		if (ft_strncmp(file->delim, result, ft_strlen(file->delim)) == 0)
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
