/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calcul_float.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmerieux <hmerieux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/12 18:45:15 by hmerieux          #+#    #+#             */
/*   Updated: 2020/01/12 18:45:17 by hmerieux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "libft.h"

long double		ft_pow(long double n, int pow)
{
	return (pow ? n * ft_pow(n, pow - 1) : 1);
}

static void		ft_convert_to_char(t_float *flt, t_bt_flt *bft, t_flags *flags)
{
	int		i;
	int		j;
	char	*notation;
	char	*exponent;

	i = 0;
	j = 0;
	notation = " * 2^";
	exponent = ft_itoa(bft->e);
	flt->sipart = ft_uitoa_base((unsigned long long)flt->ipart, 10);
	if (!(flt->sfpart = ft_strnew(flags->preci + 11)))
		return ;
	while (i < flags->preci)
	{
		flt->fpart *= 10;
		flt->sfpart[i++] = ((int)flt->fpart + 48);
		flt->fpart -= (int)flt->fpart;
	}
	while (i < flags->preci + 5)
		flt->sfpart[i++] = notation[j++];
	j = 0;
	while (exponent[j])
		flt->sfpart[i++] = exponent[j++];
	ft_strdel(&exponent);
}

void			ft_ldouble(t_float *flt, t_flags *flags)
{
	int			i;
	int			s;
	long double r;
	t_bt_flt	bft;

	ft_memcpy(&bft, &flt->ipart, sizeof(bft));
	i = 0;
	s = 62;
	bft.e -= 16383;
	r = 1;
	flt->s = (bft.s == 1) ? -1 : 1;
	while (s-- >= 0 && ++i)
		if ((bft.m >> s) & 1)
			r += ft_pow(0.5, i);
	flt->ipart = r;
	flt->fpart = flt->ipart - (unsigned long)flt->ipart;
	flt->ipart -= flt->fpart;
	ft_convert_to_char(flt, &bft, flags);
}

static char		*grap_fpart(t_flags *flags, t_float *flt)
{
	char	*s;
	int		i;

	i = 0;
	if (!(s = ft_strnew(flags->preci + 1)))
		return (NULL);
	while (i < flags->preci + 1)
	{
		flt->fpart *= 10;
		s[i] = ((int)flt->fpart + 48);
		flt->fpart -= (int)flt->fpart;
		i++;
	}
	if (s[--i] >= '5')
		while (i-- > 0)
		{
			s[i] += 1;
			if (!(s[i] == 58))
				break ;
			s[i] = '0';
		}
	s[flags->preci] = '\0';
	if (s[0] == '0' && i == -1)
		flt->ipart += 1;
	return (s);
}

void			ft_double(t_float *flt, t_flags *flags)
{
	flt->s += (flt->ipart < 0) ? -1 : 1;
	flt->ipart *= flt->s;
	flt->fpart = flt->ipart - (unsigned long long)flt->ipart;
	flt->ipart -= flt->fpart;
	flt->sfpart = grap_fpart(flags, flt);
	flt->sipart = ft_uitoa_base((unsigned long long)flt->ipart, 10);
}
