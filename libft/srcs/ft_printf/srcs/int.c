/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   int.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guaubret <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 19:20:47 by guaubret          #+#    #+#             */
/*   Updated: 2019/08/19 21:02:04 by guaubret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static intmax_t		get_int_val(t_printf *data)
{
	intmax_t	val;

	if (CH_L(data->flags))
		val = (intmax_t)(va_arg(data->ap, long));
	else if (CH_LL(data->flags))
		val = (intmax_t)va_arg(data->ap, long long);
	else if (CH_Z(data->flags))
		val = (intmax_t)va_arg(data->ap, ssize_t);
	else if (CH_J(data->flags))
		val = va_arg(data->ap, intmax_t);
	else if (CH_H(data->flags))
		val = (intmax_t)((short int)va_arg(data->ap, int));
	else if (CH_HH(data->flags))
		val = (intmax_t)((char)va_arg(data->ap, int));
	else
		val = (intmax_t)va_arg(data->ap, int);
	if (CH_ZERO(data->flags))
		data->prec = data->width;
	return (val);
}

static void			add_prefix(t_printf *data, intmax_t tmp,
		uintmax_t val, char *s)
{
	add_pad(data, 0);
	val = (tmp < 0) ? -tmp : tmp;
	xtoa_base(val, 10, s, data);
	CH_SPACE(data->flags) ? s[0] = ' ' : 0;
	(tmp < 0) ? s[0] = '-' : 0;
	(CH_PLUS(data->flags) && tmp >= 0) ? s[0] = '+' : 0;
}

void				conv_int(t_printf *data)
{
	char		s[200];
	uintmax_t	val;
	intmax_t	tmp;
	int			len;

	tmp = get_int_val(data);
	val = (tmp < 0) ? -tmp : tmp;
	len = (!tmp && !CH_PGIVEN(data->flags)) ? 1 : 0;
	while (val)
	{
		val /= 10;
		len++;
	}
	if ((tmp < 0 || CH_PLUS(data->flags) || CH_SPACE(data->flags))
	&& CH_ZERO(data->flags))
		(data->prec)--;
	data->printed = (len > data->prec) ? len : data->prec;
	(tmp < 0 || CH_PLUS(data->flags) || CH_SPACE(data->flags))
	? (data->printed)++ : 0;
	data->pad = (data->printed > data->width) ? 0 : data->width - data->printed;
	add_prefix(data, tmp, val, s);
	buffer(s, data, data->printed);
	add_pad(data, 1);
}
