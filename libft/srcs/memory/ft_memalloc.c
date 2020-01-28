/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memalloc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmerieux <hmerieux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/12 18:48:55 by hmerieux          #+#    #+#             */
/*   Updated: 2020/01/12 18:48:56 by hmerieux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

void	*ft_memalloc(size_t size)
{
	void	*s;
	int		i;

	i = 0;
	s = (void *)malloc(sizeof(s) * (int)size);
	if (!s)
		return (NULL);
	while (i < (int)size)
	{
		((int *)s)[i] = 0;
		i++;
	}
	return (s);
}
