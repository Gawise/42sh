/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dlstdel.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmerieux <hmerieux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/12 18:47:58 by hmerieux          #+#    #+#             */
/*   Updated: 2020/02/15 15:56:31 by ambelghi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

void	ft_dlstdel(t_dlist **alst)
{
	t_dlist	*tmp;
	t_dlist	*tmp2;

	if (alst && (tmp = *alst))
	{
		while (tmp->prev)
			tmp = tmp->prev;
		while (tmp)
		{
			tmp2 = tmp->next;
			if (tmp2)
				tmp2->prev = NULL;
			ft_memdel((void **)&tmp->data);
			ft_memdel((void **)&tmp);
			tmp = tmp2;
		}
		alst = NULL;
	}
}
