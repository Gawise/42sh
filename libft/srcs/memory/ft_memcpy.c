/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmerieux <hmerieux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/12 18:49:09 by hmerieux          #+#    #+#             */
/*   Updated: 2020/01/12 18:49:10 by hmerieux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	char		*d2;
	const char	*s2;

	d2 = dst;
	s2 = src;
	while (n--)
		*d2++ = *s2++;
	return (dst);
}
