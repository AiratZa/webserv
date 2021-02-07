/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdrake <gdrake@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/01 23:09:17 by gdrake            #+#    #+#             */
/*   Updated: 2020/05/02 19:11:06 by gdrake           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char *last_pos_c;

	last_pos_c = NULL;
	while (*s)
	{
		if (*s == c)
			last_pos_c = (char *)s;
		s++;
	}
	if (last_pos_c)
		return (last_pos_c);
	else if (c == '\0')
		return ((char *)s);
	else
		return (NULL);
}
