/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert_float.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmerieux <hmerieux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/12 18:45:47 by hmerieux          #+#    #+#             */
/*   Updated: 2020/01/12 18:45:49 by hmerieux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "libft.h"
#define MAX 18446744073709551615.0F

static void		ft_feed_buf(char c, int len, t_buf *buf)
{
	if (buf->j + len > 4090)
		ft_printreset(buf);
	while (len-- > 0)
		buf->buf[buf->j++] = c;
}

static int		ft_adjustflag_float(t_flags *flags, t_float *flt, int len)
{
	if (!flags->preci)
		len--;
	if (flt->s == -1)
	{
		flags->space = 0;
		flags->plus = 0;
		len += 1;
	}
	flags->large = (flags->large - len >= 0) ? (flags->large - len) : 0;
	if (flags->space == 1 && flags->plus == 1)
		flags->space = 0;
	if (flags->zero == 1 && flags->left == 1)
		flags->zero = 0;
	if ((flags->space == 1 || flags->plus == 1) && flags->large > 0)
		flags->large -= 1;
	if (flags->precinull && !flags->preci)
		ft_strdel(&flt->sfpart);
	if (flags->diez && flags->preci == 0)
		flags->large -= 1;
	if (flags->zero)
	{
		flags->zero = flags->large;
		flags->large = 0;
	}
	return (flags->left = (flags->large > 0) ? flags->left : 0);
}

static void		ft_left_feedfloat_buf(t_buf *buf, t_flags *flags, t_float *flt)
{
	int		i;

	i = 0;
	if (flags->plus == 1)
		buf->buf[buf->j++] = '+';
	if (flags->space == 1)
		buf->buf[buf->j++] = ' ';
	if (flt->s == -1)
		buf->buf[buf->j++] = '-';
	if (buf->j + ft_strlen(flt->sipart) + ft_strlen(flt->sfpart) > 4090)
		ft_printreset(buf);
	while (flt->sipart[i])
		buf->buf[buf->j++] = flt->sipart[i++];
	i = 0;
	if (flt->sfpart || flags->diez)
		buf->buf[buf->j++] = '.';
	while (flt->sfpart && flt->sfpart[i])
		buf->buf[buf->j++] = flt->sfpart[i++];
	if (flags->large)
		ft_feed_buf(' ', flags->large, buf);
}

static void		ft_right_feedfloat_buf(t_buf *buf, t_flags *flags, t_float *flt)
{
	int		i;

	i = 0;
	if (flags->large)
		ft_feed_buf(' ', flags->large, buf);
	if (flags->plus == 1)
		buf->buf[buf->j++] = '+';
	if (flags->space == 1)
		buf->buf[buf->j++] = ' ';
	if (flt->s == -1)
		buf->buf[buf->j++] = '-';
	if (flags->zero)
		ft_feed_buf('0', flags->zero, buf);
	if (buf->j + ft_strlen(flt->sipart) + ft_strlen(flt->sfpart) > 4090)
		ft_printreset(buf);
	while (flt->sipart && flt->sipart[i])
		buf->buf[buf->j++] = flt->sipart[i++];
	i = 0;
	if (flt->sfpart || flags->diez)
		buf->buf[buf->j++] = '.';
	while (flt->sfpart && flt->sfpart[i])
		buf->buf[buf->j++] = flt->sfpart[i++];
}

void			ft_convert_float(void *buf, const char *convert, va_list ap)
{
	t_float		flt;
	t_flags		flags;

	flt.s = 0;
	ft_init_structflags(&flags);
	flags.preci = 6;
	ft_grap_flags(&flags, convert, ap, -1);
	if (ft_strstr(convert, "L"))
		flt.ipart = va_arg(ap, long double);
	else
		flt.ipart = va_arg(ap, double);
	if (flt.ipart >= MAX || flt.ipart <= (MAX * -1))
		ft_ldouble(&flt, &flags);
	else
		ft_double(&flt, &flags);
	if (ft_adjustflag_float(&flags, &flt,
				(ft_strlen(flt.sfpart) + ft_strlen(flt.sipart) + 1)))
		ft_left_feedfloat_buf(buf, &flags, &flt);
	else
		ft_right_feedfloat_buf(buf, &flags, &flt);
	ft_strdel(&flt.sipart);
	ft_strdel(&flt.sfpart);
}
