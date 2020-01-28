/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utools.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmerieux <hmerieux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/12 18:46:54 by hmerieux          #+#    #+#             */
/*   Updated: 2020/01/12 18:46:56 by hmerieux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "ft_printf.h"
#include "libft.h"

void	uaux(unsigned long long n, int b, char *snb, int *p)
{
	char	*base;
	int		abs;

	base = "0123456789ABCDEF";
	if ((unsigned long long)b <= n)
		uaux(n / b, b, snb, p);
	abs = n % b;
	abs *= abs < 0 ? -1 : 1;
	snb[(*p)++] = base[abs];
}

int		ucountnb(unsigned long long value, int base)
{
	int			i;

	i = 0;
	while (value > (unsigned long long)base)
	{
		value /= base;
		i++;
	}
	return (i);
}

char	*ft_uitoa_base(unsigned long long value, int base)
{
	char	*snb;
	int		p;
	int		count;

	if (base < 2 || 16 < base)
		return (NULL);
	count = ucountnb(value, base);
	if (!(snb = (char *)malloc(sizeof(char) * count + 2)))
		return (NULL);
	p = 0;
	uaux(value, base, snb, &p);
	snb[p] = '\0';
	return (snb);
}

char	*ft_grap_unb(va_list ap, const char *convert, int base)
{
	unsigned long long nb;

	nb = 0;
	if (ft_strstr(convert, "hh"))
		nb = (unsigned long long)va_arg(ap, unsigned int);
	else if (ft_strstr(convert, "h"))
		nb = (unsigned long long)va_arg(ap, unsigned int);
	else if (ft_strstr(convert, "ll"))
		nb = (unsigned long long)va_arg(ap, unsigned long long);
	else if (ft_strstr(convert, "l") || ft_strstr(convert, "U"))
		nb = (unsigned long long)(va_arg(ap, unsigned long));
	else
		nb = (unsigned long long)va_arg(ap, unsigned int);
	return (ft_uitoa_base(nb, base));
}

char	*ft_grap_pnb(va_list ap, t_flags flags)
{
	unsigned long long	nb;
	char				*snb;

	nb = (unsigned long long)va_arg(ap, void *);
	snb = ft_uitoa_base(nb, 16);
	if (nb == 0)
		if (flags.precinull && !flags.preci)
			snb[0] = '\0';
	nb = -1;
	while (snb && snb[++nb])
		if (snb[nb] >= 'A' && snb[nb] <= 'Z')
			snb[nb] += 32;
	return (snb);
}
