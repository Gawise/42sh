/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/10 15:09:55 by user42            #+#    #+#             */
/*   Updated: 2020/07/10 15:09:55 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	char		*d2;
	const char	*s2;

	d2 = dst;
	s2 = src;
	if (src < dst)
	{
		while (len--)
			d2[len] = s2[len];
	}
	else
		while (len--)
			*d2++ = *s2++;
	return (dst);
}
