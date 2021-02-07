/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdrake <gdrake@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/04 14:02:03 by gdrake            #+#    #+#             */
/*   Updated: 2020/05/04 15:09:59 by gdrake           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list		*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*map;
	t_list	*now;
	t_list	*next;

	if (!lst || !f || !del)
		return (NULL);
	map = ft_lstnew((*f)(lst->content));
	if (map == NULL)
		return (NULL);
	now = map;
	lst = lst->next;
	while (lst)
	{
		next = ft_lstnew((*f)(lst->content));
		if (!next)
		{
			ft_lstclear(&map, del);
			return (NULL);
		}
		now->next = next;
		now = next;
		lst = lst->next;
	}
	return (map);
}
