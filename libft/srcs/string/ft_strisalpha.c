/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strisalpha.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ambelghi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/18 19:59:05 by ambelghi          #+#    #+#             */
/*   Updated: 2020/06/18 20:00:47 by ambelghi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strisalpha(char *s)
{
	int i;

	i = 0;
	while (s && s[i])
		if (!ft_isalpha(s[i]))
			return (0);
	return (1);
}
