/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/10 15:09:59 by user42            #+#    #+#             */
/*   Updated: 2020/07/10 15:09:59 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

char	*ft_strchr(char const *s, int c)
{
	int	i;

	if (s)
	{
		i = 0;
		while (s[i] != (char)c && s[i] != '\0')
			i++;
		if (s[i] == (char)c)
			return ((char *)&s[i]);
	}
	return (NULL);
}
