
#include "minishell.h"

int	is_blank(t_list *node)
{
	if (!node || !node->content)
		return (0);
	return (((t_token *)node->content)->lexem == BLANK);
}

void	remove_space(t_shell *shell, t_list *current)
{
	if (catch_error(shell) || !current->next || !is_blank(current->next))
		return ;
	ft_lstpop(&shell->token_list, current->next, free_token);
}

void	merge_token(t_shell *shell, t_list *start)
{
	t_token	*current;
	t_token	*next;

	current = ((t_token *)start->content);
	next = ((t_token *)start->next->content);
	current->content = ft_strjoinfree(current->content, next->content);
	if (!current->content)
		return (set_error(MALLOC_FAIL, shell, "Failed to malloc token"));
	ft_lstpop(&shell->token_list, start->next, free_token);
}

void	group_words(t_shell *shell, t_list *node)
{
	if (!node->next || !node->next->content || ((t_token *)node->next->content)->lexem == END)
		return ;
	while (((t_token *)node->content)->lexem == WORD
		&& ((t_token *)node->next->content)->lexem == WORD)
		merge_token(shell, node);
	while (((t_token *)node->content)->lexem == BLANK
		&& ((t_token *)node->next->content)->lexem == BLANK)
		merge_token(shell, node);
}

void	group_strings(t_shell *shell, t_list *node)
{
	t_token	*first_delim;

	first_delim = ((t_token *)node->content);
	if (first_delim->lexem != DELIMITER)
		return ;
	first_delim->content = ft_calloc(1, sizeof(char));
	if (!first_delim->content)
		return (set_error(MALLOC_FAIL, shell, "Failed to malloc string"));
	while (((t_token *)node->next->content)->lexem != END)
	{
		if (((t_token *)node->next->content)->letter == first_delim->letter)
		{
			ft_lstpop(&shell->token_list, node->next, free_token);
			break ;
		}
		merge_token(shell, node);
	}
	first_delim->lexem = STRING;
}

void	scan(t_shell *shell)
{
	shell->index = 0;
	add_token(shell, START, '\0');
	while (shell->index < ft_strlen(shell->cmd_line))
	{
		if (ft_strchr(DELIMITERS, shell->cmd_line[shell->index]))
			add_token(shell, DELIMITER, shell->cmd_line[shell->index]);
		else if (ft_strchr(OPERATORS, shell->cmd_line[shell->index]))
			add_token(shell, OPERATOR, shell->cmd_line[shell->index]);
		else if (ft_strchr(BLANKS, shell->cmd_line[shell->index]))
			add_token(shell, BLANK, shell->cmd_line[shell->index]);
		else
			add_token(shell, WORD, shell->cmd_line[shell->index]);
		shell->index++;
	}
	add_token(shell, END, '\n');
	apply_to_list(shell, shell->token_list, group_words);
	apply_to_list(shell, shell->token_list, group_strings);
	apply_to_list(shell, shell->token_list, remove_space);
}
