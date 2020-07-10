/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hash_get_node.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/10 15:09:47 by user42            #+#    #+#             */
/*   Updated: 2020/07/10 15:09:47 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_hash_node	*ft_hash_get_node(t_hash_map *map, char *key)
{
	t_list		*lst;
	t_hash_node	*node;

	lst = ft_hash_get_list(map, key);
	while (lst)
	{
		node = (t_hash_node *)lst->data;
		if (node && ft_strequ(node->key, key))
			return (node);
		lst = lst->next;
	}
	return (NULL);
}
