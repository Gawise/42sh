/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnequ.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmerieux <hmerieux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/12 18:52:23 by hmerieux          #+#    #+#             */
/*   Updated: 2020/01/12 18:52:24 by hmerieux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

int		ft_strnequ(char const *s1, char const *s2, size_t n)
{
	while (n && (*s1 == *s2) && *s1 && *s2)
	{
		s1++;
		s2++;
		n--;
	}
	if (*s1 != *s2 && (int)n > 0)
		return (0);
	else
		return (1);
}
