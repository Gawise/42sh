/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   float.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guaubret <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 19:20:43 by guaubret          #+#    #+#             */
/*   Updated: 2019/08/21 18:20:03 by guaubret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static double	get_float_val(t_printf *data)
{
	double		val;

	if (CH_LL(data->flags))
		val = (double)va_arg(data->ap, long double);
	else
		val = (double)va_arg(data->ap, double);
	return (val);
}

void			conv_float(t_printf *data)
{
	char		res[CVTBUFSZ];
	char		s[CVTBUFSZ];
	double		val;
	int			i;

	val = get_float_val(data);
	ft_ftoa(val, (CH_PGIVEN(data->flags) ? data->prec : -1), res);
	i = 0;
	ft_bzero((void *)s, CVTBUFSZ);
	CH_SPACE(data->flags) ? s[0] = ' ' : 0;
	(CH_PLUS(data->flags) && val >= 0) ? s[0] = '+' : 0;
	(s[0]) ? i = 1 : 0;
	data->pad = data->width - ft_strlen(res) + i;
	add_pad(data, 0);
	ft_strcat(s, res);
	buffer(s, data, ft_strlen(s));
	add_pad(data, 1);
}
