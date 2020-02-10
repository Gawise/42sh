/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uint.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guaubret <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 19:21:00 by guaubret          #+#    #+#             */
/*   Updated: 2019/08/19 21:04:53 by guaubret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void			get_base(t_printf *data)
{
	if (data->conv == 'u' || data->conv == 'U')
		data->base = 10;
	else if (data->conv == 'x' || data->conv == 'X' || data->conv == 'p')
		data->base = 16;
	else if (data->conv == 'o' || data->conv == 'O')
		data->base = 8;
	else if (data->conv == 'b' || data->conv == 'B')
		data->base = 2;
}

static uintmax_t	get_uint_val(t_printf *data)
{
	uintmax_t	val;

	if (data->conv == 'p')
		val = (uintmax_t)va_arg(data->ap, void *);
	else if (CH_L(data->flags))
		val = (uintmax_t)va_arg(data->ap, unsigned long);
	else if (CH_LL(data->flags))
		val = (uintmax_t)va_arg(data->ap, unsigned long long);
	else if (CH_Z(data->flags))
		val = (uintmax_t)va_arg(data->ap, size_t);
	else if (CH_J(data->flags))
		val = va_arg(data->ap, uintmax_t);
	else if (CH_HH(data->flags))
		val = (uintmax_t)((unsigned char)va_arg(data->ap, unsigned int));
	else if (CH_H(data->flags))
		val = (uintmax_t)((short unsigned)va_arg(data->ap, unsigned int));
	else
		val = (uintmax_t)va_arg(data->ap, unsigned int);
	get_base(data);
	return (val);
}

static void			add_prefix(t_printf *data, uintmax_t val, int ext)
{
	(CH_SHARP(data->flags) && val && data->base == 8 && !ext)
	? (data->width)-- : 0;
	(CH_SHARP(data->flags) && data->base == 8 && !val
	&& CH_PGIVEN(data->flags) && !data->prec)
	? (data->printed)++ : 0;
	if ((CH_SHARP(data->flags) && data->base == 16
	&& !CH_ZERO(data->flags) && val) || CH_PCONV(data->flags))
		data->width -= 2;
	if ((data->width - data->printed) > 0)
		data->pad = data->width - data->printed;
	else
		data->pad = 0;
	add_pad(data, 0);
	if ((val || CH_PCONV(data->flags)) && CH_SHARP(data->flags)
	&& ((data->base == 8 && !ext) || data->base == 16))
		buffer("0", data, 1);
	if ((val || CH_PCONV(data->flags))
	&& CH_SHARP(data->flags) && data->base == 16)
		buffer(CH_UPX(data->flags) ? "X" : "x", data, 1);
}

void				conv_uint(t_printf *data)
{
	char		s[200];
	uintmax_t	val;
	uintmax_t	tmp;
	int			ext;

	val = get_uint_val(data);
	tmp = val;
	data->printed = 0;
	while (tmp && ++(data->printed))
		tmp /= data->base;
	CH_ZERO(data->flags) ? data->prec = data->width : 0;
	ext = (data->printed >= data->prec) ? 0 : 1;
	data->printed = (data->prec > data->printed) ? data->prec : data->printed;
	if (data->printed <= 0 && !CH_PGIVEN(data->flags))
		data->printed = 1;
	add_prefix(data, val, ext);
	xtoa_base(val, data->base, s, data);
	buffer(s, data, data->printed);
	add_pad(data, 1);
}
