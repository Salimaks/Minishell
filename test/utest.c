/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utest.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 15:58:43 by mkling            #+#    #+#             */
/*   Updated: 2024/12/04 11:57:27 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/inc/libft.h"
#include "/home/mkling/Criterion/include/criterion/criterion.h"
#include "/home/mkling/Criterion/include/criterion/redirect.h"
#include <stdio.h>

// void	*ft_calloc(size_t nbr, size_t size);

// void	redirect_all_stdout(void)
// {
// 	cr_redirect_stdout();
// 	cr_redirect_stderr();
// }

// t_cmd_tab	*create_cmd_tab(int cmd_count)
// {
// 	t_cmd_tab	*cmd_tab;

// 	cmd_tab = ft_calloc(sizeof(t_cmd_tab), 1);
// 	if (!cmd_tab)
// 		return (NULL);
// 	cmd_tab->cmd_array = ft_calloc(sizeof(t_cmd), cmd_count);
// 	if (!cmd_tab->cmd_array)
// 		return (NULL);
// 	cmd_tab->index = 0;
// 	return (cmd_tab);
// }

Test(Criterion, Setup)
{
	cr_assert_eq("meow", 0);
}


// Test(general, error_message, .init=redirect_all_stdout)
// {
// 	// exit_if(true, "test1", 0, NULL);
// 	fprintf(stderr, "AAA");
// 	cr_assert_stderr_eq_str("AAA");
// }

// /* Lexer: Tokenizing */
// Test(Lexer, Token_number)
// {
// 	t_cmd_tab	cmd_tab;

// 	parse("llo", &cmd_tab);
// 	cr_expect(cmd_tab.content[0] == 'l');
// 	parse("no", &cmd_tab);
// 	cr_expect(cmd_tab.content[0] == 'n');
// }

// /* Parser: */
// Test(Parser, Filling)
// {
// 	t_cmd_tab	cmd_tab;

// 	parse("llo", &cmd_tab);
// 	cr_expect(cmd_tab.content[0] == 'l');
// 	parse("no", &cmd_tab);
// 	cr_expect(cmd_tab.content[0] == 'n');
// }
