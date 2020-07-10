/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdelhead.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/10 15:09:50 by user42            #+#    #+#             */
/*   Updated: 2020/07/10 15:09:50 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstdelhead(t_list **alst, void (*del)(void*, size_t))
{
	t_list	*item;

	item = (*alst)->next;
	ft_lstdelone(alst, del);
	*alst = item;
}
