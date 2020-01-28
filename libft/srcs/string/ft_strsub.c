/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsub.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmerieux <hmerieux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/12 18:53:01 by hmerieux          #+#    #+#             */
/*   Updated: 2020/01/12 18:53:01 by hmerieux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include "../../includes/libft.h"

char	*ft_strsub(char const *s, unsigned int start, size_t len)
{
	char	*dst;
	int		i;

	i = 0;
	dst = ft_strnew(len);
	if (!dst)
		return (NULL);
	while (len > 0)
	{
		dst[i] = s[start];
		i++;
		start++;
		len--;
	}
	dst[i] = '\0';
	return (dst);
}
