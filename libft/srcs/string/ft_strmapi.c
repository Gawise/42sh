/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmerieux <hmerieux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/12 18:52:08 by hmerieux          #+#    #+#             */
/*   Updated: 2020/01/12 18:52:09 by hmerieux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "../../includes/libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char		*dst;
	size_t		i;

	i = 0;
	dst = (char *)malloc(sizeof(char) * ft_strlen(s) + 1);
	if (!dst)
		return (NULL);
	while (i < ft_strlen(s))
	{
		dst[i] = f(i, s[i]);
		i++;
	}
	dst[i] = '\0';
	return (dst);
}
