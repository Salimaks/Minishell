/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skassimi <skassimi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 17:13:45 by skassimi          #+#    #+#             */
/*   Updated: 2023/11/21 19:13:18 by skassimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *nbr)
{
	int		sign;
	int		resultat;
	char	*nb;

	sign = 1;
	resultat = 0;
	nb = (char *)nbr;
	while ((*nb >= 9 && *nb <= 13) || *nb == 32)
		nb++;
	if (*nb == '+' || *nb == '-')
	{
		if (*nb == '-')
			sign *= -1;
		nb++;
	}
	while (*nb != '\0' && (*nb >= '0' && *nb <= '9'))
	{
		resultat = resultat * 10 + (*nb - 48);
		nb++;
	}
	return (resultat * sign);
}
