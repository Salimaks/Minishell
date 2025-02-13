/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skassimi <skassimi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 21:06:32 by skassimi          #+#    #+#             */
/*   Updated: 2025/01/17 17:48:56 by skassimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void handle_redirection(t_token **tokens, t_command *cmd) {
    if (!(*tokens)->next)
        return;
    if ((*tokens)->type == T_REDIR_IN)
        cmd->input_file = strdup((*tokens)->next->value);
    else if ((*tokens)->type == T_REDIR_OUT) {
        cmd->output_file = strdup((*tokens)->next->value);
        cmd->append = 0;
    } else if ((*tokens)->type == T_REDIR_APPEND) {
        cmd->output_file = strdup((*tokens)->next->value);
        cmd->append = 1;
    }
    *tokens = (*tokens)->next;
}
