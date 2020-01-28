/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmerieux <hmerieux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/12 18:52:04 by hmerieux          #+#    #+#             */
/*   Updated: 2020/01/12 18:52:05 by hmerieux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "../../includes/libft.h"

char	*ft_strmap(char const *s, char (*f)(char))
{
	char		*dst;
	size_t		i;

	i = 0;
	dst = (char *)malloc(sizeof(char) * ft_strlen(s) + 1);
	if (!dst)
		return (NULL);
	while (i < ft_strlen(s))
	{
		dst[i] = f(s[i]);
		i++;
	}
	dst[i] = '\0';
	return (dst);
}
