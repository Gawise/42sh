/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstpushfront.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmerieux <hmerieux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/12 18:48:40 by hmerieux          #+#    #+#             */
/*   Updated: 2020/01/12 18:48:41 by hmerieux         ###   ########.fr       */
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
