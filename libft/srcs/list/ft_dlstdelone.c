/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dlstdelone.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmerieux <hmerieux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/12 18:48:07 by hmerieux          #+#    #+#             */
/*   Updated: 2020/04/10 20:30:31 by guaubret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

void	ft_dlstdelone(t_dlist **alst)
{
	t_dlist *tmp;
	t_dlist *prev;
	t_dlist	*next;

	if (alst && (tmp = *alst))
	{
		prev = tmp->prev;
		next = tmp->next;
		if (next)
			next->prev = prev;
		if (prev)
			prev->next = next;
		ft_memdel((void **)&tmp->data);
		ft_memdel((void **)&tmp);
		alst = NULL;
	}
}
