/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skassimi <skassimi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 14:09:43 by mkling            #+#    #+#             */
/*   Updated: 2024/12/27 11:27:30 by skassimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


int	main(int argc, char **argv)
{
	if (argc < 2) 
	{
        return (0);
	}
    t_list *tokens = tokenizer(argv[1]);
    if (!tokens) {
        printf("Erreur : impossible de tokenizer l'entrée\n");
        return (0);
    }
    printf(" Tokens\n");
    print_tokens(tokens);
    free_tokens(tokens);
    return (0);
}
