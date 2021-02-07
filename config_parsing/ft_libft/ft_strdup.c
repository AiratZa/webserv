/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdrake <gdrake@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/02 19:59:00 by gdrake            #+#    #+#             */
/*   Updated: 2020/05/03 22:41:03 by gdrake           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

static char	*ft_strcpy(char *dest, const char *src)
{
	int i;

	i = 0;
	while (src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char		*ft_strdup(const char *s)
{
	char	*new_arr;

	new_arr = (char *)malloc((ft_strlen(s) + 1) * sizeof(char));
	if (new_arr == NULL)
		return (NULL);
	else
		return ((char *)ft_strcpy(new_arr, s));
}
