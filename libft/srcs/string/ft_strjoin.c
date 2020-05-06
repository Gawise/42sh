/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmerieux <hmerieux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/12 18:51:43 by hmerieux          #+#    #+#             */
/*   Updated: 2020/01/12 18:51:45 by hmerieux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdarg.h>
#include <stdlib.h>
#include "libft.h"

static	int		find_len(va_list ap, int nb)
{
	va_list	ap2;
	int		len;
	char	*s;

	len = 0;
	va_copy(ap2, ap);
	while (nb--)
	{
		s = va_arg(ap2, char *);
		len += ft_strlen(s);
	}
	va_end(ap2);
	return (len);
}

char			*ft_strjoin(int nb, ...)
{
	va_list	ap;
	char	*dst;
	int		len;
	char	*tmp;

	va_start(ap, nb);
	len = find_len(ap, nb);
	if (!(dst = ft_strnew(len)))
		exit(EXIT_FAILURE);
	while (nb--)
		if ((tmp = va_arg(ap, char *)))
			ft_strcat(dst, tmp);
	va_end(ap);
	return (dst);
}
