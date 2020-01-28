/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmerieux <hmerieux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/12 18:51:25 by hmerieux          #+#    #+#             */
/*   Updated: 2020/01/15 17:22:47 by hmerieux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_strdup(const char *s1)
{
	char	*dest;
	int		i;

	i = -1;
	if (!s1)
		return (NULL);
	if (!(dest = (char*)malloc(sizeof(char) * ft_strlen(s1) + 1)))
		exit(EXIT_FAILURE);
	while (s1[++i])
		dest[i] = s1[i];
	dest[i] = '\0';
	return (dest);
}
