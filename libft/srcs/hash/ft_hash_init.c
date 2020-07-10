/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hash_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/10 15:09:47 by user42            #+#    #+#             */
/*   Updated: 2020/07/10 15:09:47 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_hash_map	*ft_hash_init(uint32_t size)
{
	t_hash_map	*map;

	if (size < 1)
		return (NULL);
	if (!(map = (t_hash_map *)ft_memalloc(sizeof(t_hash_map))))
		return (NULL);
	map->size = size;
	if (!(map->slots = (t_list **)ft_memalloc((size + 1) * sizeof(t_list *))))
		return (NULL);
	return (map);
}
