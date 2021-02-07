/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdrake <gdrake@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/03 15:49:42 by gdrake            #+#    #+#             */
/*   Updated: 2020/05/03 23:24:34 by gdrake           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void		ft_putnbr_fd(int n, int fd)
{
	char	nbr[10];
	int		i;

	i = 0;
	if (n == 0)
		ft_putchar_fd('0', fd);
	if (n < 0)
	{
		ft_putchar_fd('-', fd);
		if (n == -2147483648)
		{
			ft_putchar_fd('2', fd);
			n = (n % 1000000000);
		}
		n *= -1;
	}
	while (n != 0)
	{
		nbr[i] = ((n % 10) + '0');
		n = n / 10;
		i++;
	}
	while (--i >= 0)
		ft_putchar_fd(nbr[i], fd);
}
