/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmerieux <hmerieux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/12 18:46:45 by hmerieux          #+#    #+#             */
/*   Updated: 2020/01/12 18:53:37 by hmerieux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "ft_printf.h"
#include "libft.h"

void	aux(long long n, int b, char *snb, int *p)
{
	int		abs;
	char	*base;

	base = "0123456789ABCDEF";
	if (n <= -b || b <= n)
		aux(n / b, b, snb, p);
	abs = n % b;
	abs *= abs < 0 ? -1 : 1;
	snb[(*p)++] = base[abs];
}

char	*ft_itoa_base(long long value, int base)
{
	char		*snb;
	int			p;
	long long	tmp;
	int			count;

	p = 0;
	tmp = value;
	count = 0;
	if (base < 2 || 16 < base)
		return (NULL);
	while (tmp > base || tmp < (base * -1))
	{
		tmp /= base;
		count++;
	}
	if (!(snb = (char *)malloc(sizeof(char) * count + 3)))
		return (NULL);
	if (base == 10 && value < 0)
		snb[p++] = '-';
	aux(value, base, snb, &p);
	snb[p] = '\0';
	return (snb);
}

int		ft_atoi_convert(int *i, const char *convert)
{
	int		large;

	large = 0;
	if (convert[*i] == '.')
		*i += 1;
	while ((convert[*i] >= '0' && convert[*i] <= '9') && convert[*i])
	{
		large = large * 10 + (convert[*i] - 48);
		(*i)++;
	}
	*i -= 1;
	return (large);
}

char	*ft_grap_nb(va_list ap, const char *convert)
{
	long long nb;

	nb = 0;
	if (ft_strstr(convert, "hh"))
		nb = (signed char)va_arg(ap, int);
	else if (ft_strstr(convert, "h"))
		nb = (short)va_arg(ap, int);
	else if (ft_strstr(convert, "ll"))
		nb = (long long)va_arg(ap, long long);
	else if (ft_strstr(convert, "l"))
		nb = (long)(va_arg(ap, long));
	else if (ft_strstr(convert, "D"))
		nb = (long)(va_arg(ap, long));
	else
		nb = (int)va_arg(ap, int);
	return (ft_itoa_base(nb, 10));
}

void	ft_grap_flags(t_flags *flags, const char *convert, va_list ap, int i)
{
	while (convert[++i] && convert[i])
	{
		if (convert[i] == '*' && convert[i - 1] != '.')
			flags->large = (int)va_arg(ap, int);
		else if (convert[i] == '-')
			flags->left = 1;
		else if (convert[i] == '0')
			flags->zero = 1;
		else if (convert[i] > '0' && convert[i] <= '9')
			flags->large = ft_atoi_convert(&i, convert);
		else if (convert[i] == '.')
		{
			flags->precinull = 1;
			if (convert[i + 1] == '*')
				flags->preci = (int)va_arg(ap, int);
			else
				flags->preci = ft_atoi_convert(&i, convert);
		}
		else if (convert[i] == '+')
			flags->plus = 1;
		else if (convert[i] == ' ')
			flags->space = 1;
		else if (convert[i] == '#')
			flags->diez = 1;
	}
}
