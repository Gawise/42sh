/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hash_lookup.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/10 15:09:48 by user42            #+#    #+#             */
/*   Updated: 2020/07/10 15:09:48 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void		*ft_hash_lookup(t_hash_map *map, char *key)
{
	t_hash_node	*node;

	if (!(node = ft_hash_get_node(map, key)))
		return (NULL);
	return (node->data);
}
