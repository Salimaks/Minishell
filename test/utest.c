/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utest.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 15:58:43 by mkling            #+#    #+#             */
/*   Updated: 2024/11/29 12:23:13 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../include/libft/inc/libft.h"
#include "criterion/criterion.h"
#include "criterion/redirect.h"
#include <stdio.h>

// void	*ft_calloc(size_t nbr, size_t size);

// void	redirect_all_stdout(void)
// {
// 	cr_redirect_stdout();
// 	cr_redirect_stderr();
// }

void	ft_bzero(void *ptr, unsigned long len)
{
	unsigned char	*str;

	str = ptr;
	while (len--)
		*str++ = 0;
}

void	*ft_calloc(size_t nbr, size_t size)
{
	void	*result;

	if (nbr == 0 || size == 0 || ((size_t) - 1 / nbr) < size)
		return (cr_malloc(0));
	result = cr_malloc(nbr * size);
	if (!result)
		return (NULL);
	ft_bzero(result, nbr * size);
	return (result);
}

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
	cr_assert_eq("meow", "meow");
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
