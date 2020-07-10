/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/10 15:10:05 by user42            #+#    #+#             */
/*   Updated: 2020/07/10 15:10:05 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

char	*ft_strnew(size_t size)
{
	char	*str;
	int		i;

	i = (int)size;
	str = (char *)malloc(sizeof(char) * i + 1);
	if (!str)
		return (NULL);
	while (i >= 0)
	{
		str[i] = '\0';
		i--;
	}
	return (str);
}
