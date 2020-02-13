/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_wstrlen.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guaubret <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 20:40:57 by guaubret          #+#    #+#             */
/*   Updated: 2019/08/19 20:40:58 by guaubret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t		ft_wstrlen(unsigned *str)
{
	size_t	len;

	len = 0;
	while (*str != L'\0')
	{
		len += ft_wcharlen(*str);
		str++;
	}
	return (len);
}