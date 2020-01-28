/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmerieux <hmerieux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/12 18:50:49 by hmerieux          #+#    #+#             */
/*   Updated: 2020/01/12 18:50:49 by hmerieux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

char	*ft_strchr(const char *s, int c)
{
	char *str;

	str = (char *)s;
	while (str && *str != c)
	{
		if (*str == '\0')
			return (NULL);
		str++;
	}
	return (str);
}
