/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lst_join.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/10 15:09:52 by user42            #+#    #+#             */
/*   Updated: 2020/07/10 15:09:52 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lst_join(t_list *lst1, t_list **lst2, int refresh)
{
	t_list *tmp;

	if (!lst1)
	{
		lst1 = *lst2;
		return ;
	}
	tmp = lst1;
	while (lst1->next)
		lst1 = lst1->next;
	lst1->next = *lst2;
	if (refresh)
		*lst2 = tmp;
}
