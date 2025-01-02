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
	if (((t_token *)node->prev->content)->letter == '\'')
		return (0);
	return (1);
}

int	has_valid_var(char *string)
{
	char	first_letter;

	if (!string)
		return (0);
	first_letter = *(ft_strchr(string, '$') + 1);
	if (first_letter == '_' || ft_isalpha(first_letter))
		return (1);
	return (0);
}

void	expand_var_token(t_shell *shell, t_token *token)
{
	char	*var_name;
	t_list	*env;

	var_name = token->content;
	if (ft_strcmp(var_name, "$?") == 0)
		token->content = ft_itoa(shell->last_exit_code);
	else
	{
		env = find_env(shell->env_list, &var_name[1]);
		if (env)
			token->content = (char *)env->content;
		else
			token->content = NULL;
	}
	free(var_name);
}

char	*flatten_token_list_into_string(t_shell *shell, t_list *head)
{
	int		len;
	t_list	*current;
	char	*result;

	if (shell->critical_er)
		return NULL;
	current = head;
	while (current->next)
	{
		if (current->content)
			len += ft_strlen((char *)((t_token *)current->content)->content);
		current = current->next;
	}
	result = ft_calloc(sizeof(char), len + 1);
	len = 0;
	while (current)
	{
		if (current->content)
			ft_strlcat(result, (char *)((t_token *)current->content)->content,
				ft_strlen((char *)((t_token *)current->content)->content));
		current = current->next;
	}
	return (result);
}

void	expand_in_string(t_shell *shell, t_list *node)
{
	t_list	*token_list;
	t_list	*current;
	t_token	*token;

	if (!node || !node->content)
		return ;
	token_list = NULL;
	scan(shell, &token_list, (char *)node->content);
	apply_to_list(shell, token_list, id_variables);
	apply_to_list(shell, token_list, group_strings);
	current = token_list;
	while (current->next)
	{
		token = (t_token *)current->content;
		if (token->lexem == VARIABLE)
			expand_var_token(shell, token);
		if (token->lexem == STRING && token->letter != '\''
			&& has_valid_var((char *)token->content))
			fprintf(stderr, "need to scan lex and expand string\n");
		current = current->next;
	}
	print_tokens(token_list);
}

void	expand(t_shell *shell, t_list *node)
{
	char	*var_name;
	t_list	*relevant_env;

	if (!node || !node->content || !is_valid_variable((char *)node->content))
		return ;
	var_name = (char *)node->content;
	if (ft_strcmp(var_name, "$?") == 0)
		node->content = ft_itoa(shell->last_exit_code);
	else
	{
		relevant_env = find_env(shell->env_list, &var_name[1]);
		if (relevant_env)
			node->content = (char *)relevant_env->content + ft_strlen(var_name);
		else
			node->content = NULL;
	}
	free(var_name);
}
