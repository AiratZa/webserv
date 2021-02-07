/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdrake <gdrake@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/01 23:08:52 by gdrake            #+#    #+#             */
/*   Updated: 2020/05/03 21:31:28 by gdrake           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	src_len;
	size_t	src_counter;
	size_t	dst_counter;

	src_counter = 0;
	dst_counter = 0;
	if (!src)
		return (0);
	src_len = ft_strlen(src);
	if (!dst || (size == 0))
		return (src_len);
	while (src[src_counter] && (src_counter < (size - 1)))
	{
		dst[dst_counter] = src[src_counter];
		src_counter++;
		dst_counter++;
	}
	dst[dst_counter] = '\0';
	return (src_len);
}
