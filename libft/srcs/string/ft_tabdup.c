/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tabdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/10 15:10:08 by user42            #+#    #+#             */
/*   Updated: 2020/07/10 15:10:08 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	**ft_tabdup(char **t)
{
	char	**dup;
	int		i;

	dup = NULL;
	if (t)
	{
		i = 0;
		while (t[i])
			i++;
		if (!(dup = (char **)malloc(sizeof(char*) * (i + 1))))
			exit(EXIT_FAILURE);
		dup[i] = NULL;
		while (i-- > 0)
			dup[i] = ft_strdup(t[i]);
	}
	return (dup);
}
