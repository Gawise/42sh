/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstpushfront.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/10 15:09:54 by user42            #+#    #+#             */
/*   Updated: 2020/07/10 15:09:54 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lst_push_front(t_list **begin, void *content, size_t content_size)
{
	t_list	*new;

	if (!(new = ft_lstnew(content, content_size)))
		return ;
	if (*begin)
		new->next = *begin;
	*begin = new;
}
