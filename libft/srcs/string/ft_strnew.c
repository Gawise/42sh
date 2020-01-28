/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmerieux <hmerieux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/12 18:52:26 by hmerieux          #+#    #+#             */
/*   Updated: 2020/01/12 18:52:26 by hmerieux         ###   ########.fr       */
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
