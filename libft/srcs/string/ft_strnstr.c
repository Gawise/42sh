/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmerieux <hmerieux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/12 18:52:28 by hmerieux          #+#    #+#             */
/*   Updated: 2020/01/12 18:52:29 by hmerieux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

char	*ft_strnstr(const char *h, const char *n, size_t len)
{
	unsigned long		i;
	unsigned int		r;
	unsigned int		j;

	i = 0;
	if (n[0] == '\0')
		return ((char *)h);
	while (h[i] && i < len)
	{
		j = 0;
		if (h[i] == n[j])
		{
			r = i;
			while (h[i + j] == n[j] && h[i + j] && n[j]
					&& (i + j) < len)
				j++;
			if (n[j] == '\0')
				return ((char *)&h[r]);
		}
		i++;
	}
	return (NULL);
}
