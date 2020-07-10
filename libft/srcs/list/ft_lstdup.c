/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/10 15:09:51 by user42            #+#    #+#             */
/*   Updated: 2020/07/10 15:09:51 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static t_list	*dup_item(t_list *lst, size_t size,
							void (*cplst)(void *, void *))
{
	t_list	*new;

	if (!(new = ft_lstnew(0, 0))
	|| !(new->data = ft_memalloc(size)))
		return (NULL);
	new->size = size;
	cplst(new->data, lst->data);
	return (new);
}

t_list			*ft_lstdup(t_list *lst, size_t size,
							void (*cplst)(void *, void *))
{
	t_list	*new;
	t_list	*ret;

	if (!lst)
		return (ft_lstnew(0, 0));
	if (!(new = dup_item(lst, size, cplst)))
		return (NULL);
	ret = new;
	lst = lst->next;
	while (lst)
	{
		if (!(new->next = dup_item(lst, size, cplst)))
			return (NULL);
		lst = lst->next;
		new = new->next;
	}
	new->next = NULL;
	return (ret);
}
