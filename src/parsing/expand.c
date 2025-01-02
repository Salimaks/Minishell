/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 16:54:16 by mkling            #+#    #+#             */
/*   Updated: 2024/12/30 17:08:45 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


// tokenize "" or '' as single empty string to ensure ""'$USER'"" and ''$USER''
// expand as they do in bash
/* Applies requirements for expansion,
NEEDS TO BE ADAPTED */
int	can_expand(t_list *node)
{
	char	*string;

	if (!node || !node->content)
		return (0);
	string = (char *)node->content;
	if (string[0] == '\'')
		return (0);
	return (1);
}

int	has_valid_var(char *string)
{
	char	first_letter;

	if (!string || !string[0] || !ft_strchr(string, '$'))
		return (0);
	first_letter = *(ft_strchr(string, '$') + 1);
	if (first_letter == '_' || ft_isalpha(first_letter))
		return (1);
	return (0);
}

void	expand_variable(t_shell *shell, char **ptr_to_variable)
{
	char	*var_name;
	char	*expanded_var;
	t_list	*relevant_env;

	var_name = (char *)(*ptr_to_variable);
	if (ft_strcmp(var_name, "$?") == 0)
		expanded_var = ft_itoa(shell->last_exit_code);
	else
	{
		relevant_env = find_env(shell->env_list, &var_name[1]);
		if (relevant_env)
			expanded_var = (char *)relevant_env->content + ft_strlen(var_name);
		else
			expanded_var = NULL;
	}
	free(var_name);
	*ptr_to_variable = expanded_var;
}

char	*flatten_token_list_into_string(t_shell *shell, t_list *head)
{
	int		len;
	t_list	*current;
	t_token	*token;
	char	*result;

	if (shell->critical_er)
		return NULL;
	current = head;
	len = 0;
	while (current->next)
	{
		if (current->content && ((t_token *)current->content)->content)
			len += ft_strlen((char *)((t_token *)current->content)->content);
		current = current->next;
	}
	fprintf(stderr, "len is %u\n", len);
	result = ft_calloc(sizeof(char), len + 1);
	len = 0;
	current = head;
	while (current->next)
	{
		token = (t_token *)current->content;
		fprintf(stderr, "word is %s\n", (char *)token->content);
		if (current->content && ((t_token *)current->content)->content)
			ft_strlcat(result, (char *)token->content, ft_strlen((char *)token->content));
		current = current->next;
	}
	fprintf(stderr, "string is : %s\n", result);
	return (result);
}

void	expand_in_string(t_shell *shell, t_list *node)
{
	t_list	*token_list;
	t_list	*current;
	t_token	*token;
	char	*expanded_string;

	if (!node || !node->content)
		return ;
	token_list = NULL;
	fprintf(stderr, "string is %s\n", (char *)node->content);
	scan(shell, &token_list, (char *)node->content);
	apply_to_list(shell, token_list, id_variables);
	apply_to_list(shell, token_list, group_strings);
	current = token_list;
	while (current->next)
	{
		token = (t_token *)current->content;
		if (token->lexem == VARIABLE)
			expand_variable(shell, &token->content);
		current = current->next;
	}
	expanded_string = flatten_token_list_into_string(shell, token_list);
	free(node->content);
	node->content = expanded_string;
}

void	expand(t_shell *shell, t_list *node)
{
	if (!node || !node->content)
		return ;
	if (is_valid_variable((char *)node->content))
		expand_variable(shell, (char **)&node->content);
	if (has_valid_var((char *)node->content) && can_expand(node))
	{
		remove_delimiter_on_ptr(shell, &node->content);
		expand_in_string(shell, node);
	}

}
