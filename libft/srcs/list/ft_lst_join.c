/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lst_join.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmerieux <hmerieux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/12 18:47:48 by hmerieux          #+#    #+#             */
/*   Updated: 2020/01/12 18:47:49 by hmerieux         ###   ########.fr       */
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
