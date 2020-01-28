/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmerieux <hmerieux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/12 18:48:20 by hmerieux          #+#    #+#             */
/*   Updated: 2020/01/12 18:48:21 by hmerieux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

t_list	*ft_lstmap(t_list *lst, t_list *(*f)(t_list *elem))
{
	t_list	*lstdst;
	t_list	*begin;
	t_list	*tmp;

	tmp = f(lst);
	lstdst = ft_lstnew(tmp->content, tmp->content_size);
	if (!lstdst)
		return (NULL);
	begin = lstdst;
	lst = lst->next;
	while (lst)
	{
		tmp = f(lst);
		lstdst->next = ft_lstnew(tmp->content, tmp->content_size);
		if (!lstdst)
			return (NULL);
		lstdst = lstdst->next;
		lst = lst->next;
	}
	return (begin);
}
