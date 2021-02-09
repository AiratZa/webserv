/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdrake <gdrake@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/01 23:04:28 by gdrake            #+#    #+#             */
/*   Updated: 2020/05/09 19:02:44 by gdrake           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.hpp"

int			libft::atoi(const char *nptr)
{
	long	nb;
	long	tmp;
	int		i;
	int		minus;

	nb = 0;
	i = 0;
	minus = 0;
	while (isspace((int)nptr[i]))
		i++;
	if (nptr[i] == '-')
	{
		minus = 1;
		i++;
	}
	else if (nptr[i] == '+')
		i++;
	while ((nptr[i] >= '0') && (nptr[i] <= '9'))
	{
		tmp = nb;
		nb = nb * 10 + (nptr[i++] - '0');
		if (nb < tmp)
			return ((minus) ? 0 : -1);
	}
	return ((minus) ? (int)(-nb) : (int)(nb));
}
