/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstpushback.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/10 15:09:53 by user42            #+#    #+#             */
/*   Updated: 2020/07/10 15:09:53 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lst_push_back(t_list **begin, void *content, size_t content_size)
{
	t_list	*new;
	t_list	*tmp;

	tmp = *begin;
	if (!(new = ft_lstnew(content, content_size)))
		return ;
	if (*begin)
	{
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
	else
		*begin = new;
}
