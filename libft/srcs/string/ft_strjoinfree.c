/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoinfree.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/10 15:10:03 by user42            #+#    #+#             */
/*   Updated: 2020/07/10 15:10:03 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"
#include <stdlib.h>

char	*ft_strjoinfree(char *s1, char *s2, int tofree)
{
	size_t	len;
	char	*str;

	len = ft_strlen(s1) + ft_strlen(s2);
	if (!(str = (char *)ft_memalloc(len + 1)))
		return (NULL);
	ft_strcat(str, s1);
	ft_strcat(str, s2);
	if (tofree == 1 || tofree == 3)
		free(s1);
	else if (tofree == 2 || tofree == 3)
		free(s2);
	return (str);
}
