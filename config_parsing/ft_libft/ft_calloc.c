/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdrake <gdrake@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/02 19:38:27 by gdrake            #+#    #+#             */
/*   Updated: 2020/05/03 21:14:14 by gdrake           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*arr;

	arr = (char *)malloc(nmemb * size);
	if (arr == NULL)
		return (NULL);
	else
	{
		ft_memset(arr, 0, (nmemb * size));
		return (arr);
	}
}
