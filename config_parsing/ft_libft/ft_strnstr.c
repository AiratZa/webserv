/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdrake <gdrake@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/01 23:07:42 by gdrake            #+#    #+#             */
/*   Updated: 2020/05/02 19:21:19 by gdrake           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	int b_i;
	int l_i;

	b_i = 0;
	l_i = 0;
	while (big[b_i] && (size_t)b_i < len)
	{
		if (big[b_i] == little[l_i])
		{
			while (little[l_i] && ((size_t)(b_i + l_i) < len)\
					&& (big[b_i + l_i] == little[l_i]))
				l_i++;
			if (little[l_i] == '\0')
				return ((char *)&big[b_i]);
			l_i = 0;
		}
		b_i++;
	}
	if (*little == '\0')
		return ((char *)big);
	return (NULL);
}
