/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdrake <gdrake@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/03 11:44:24 by gdrake            #+#    #+#             */
/*   Updated: 2020/05/03 23:17:11 by gdrake           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

static size_t	ft_d_count(int n)
{
	size_t		d_count;

	d_count = 1;
	if (n < 0)
		n *= -1;
	while (n >= 10)
	{
		d_count++;
		n /= 10;
	}
	return (d_count);
}

static char		*min_int_handler(int n)
{
	size_t	i;
	char	*nbr;

	i = 10;
	nbr = (char *)malloc(12 * sizeof(char));
	if (nbr == NULL)
		return (NULL);
	nbr[11] = '\0';
	n++;
	n *= -1;
	nbr[0] = '-';
	while (i > 0)
	{
		nbr[i] = (n % 10) + '0';
		n /= 10;
		i--;
	}
	nbr[10] = '8';
	return (nbr);
}

char			*ft_itoa(int n)
{
	char	*nbr;
	size_t	d_count;
	size_t	minus;

	minus = 0;
	if (n == (-2147483648))
		return (min_int_handler(n));
	d_count = ft_d_count(n);
	if (n < 0)
		minus = 1;
	nbr = (char *)malloc((d_count + minus + 1) * sizeof(char));
	if (nbr == NULL)
		return (NULL);
	nbr[d_count + minus] = '\0';
	if (n < 0)
	{
		n *= -1;
		nbr[0] = '-';
	}
	while (d_count > 0)
	{
		nbr[(d_count--) + minus - 1] = (n % 10) + '0';
		n /= 10;
	}
	return (nbr);
}
