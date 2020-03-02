/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   misc.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guaubret <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 19:20:50 by guaubret          #+#    #+#             */
/*   Updated: 2019/08/21 12:22:29 by guaubret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void			xtoa_base(uintmax_t val, int base, char s[21], t_printf *data)
{
	int		len;
	char	c;

	if (val && !CH_PCONV(data->flags) && CH_ZERO(data->flags)
	&& CH_SHARP(data->flags) && base == 16
	&& !CH_LL(data->flags) && data->printed > 3)
		data->printed -= 2;
	len = data->printed;
	c = (CH_UPX(data->flags)) ? 55 : 87;
	while (len--)
	{
		s[len] = val % base + ((val % base < 10) ? '0' : c);
		val /= base;
	}
	(CH_PGIVEN(data->flags) && CH_ZERO(data->flags)) ? s[0] = ' ' : 0;
}

static void		as_flush(t_printf *data)
{
	size_t		len;
	char		*new;

	if (!*(data->str))
	{
		if (!(*data->str = ft_strnew(data->buf_len + 1)))
		{
			data->ret = -1;
			return ;
		}
		ft_strncpy(*(data->str), data->buf, data->buf_len);
	}
	else
	{
		len = ft_strlen(*(data->str));
		if (!(new = ft_strnew(len + data->buf_len + 1)))
		{
			data->ret = -1;
			return ;
		}
		ft_strcpy(new, *(data->str));
		free(*(data->str));
		ft_strncat(new, data->buf, data->buf_len);
		*(data->str) = new;
	}
}

void			flush(t_printf *data)
{
	if (data->type == 'd' || data->type == 'p')
		write(data->fd, data->buf, data->buf_len);
	else if (data->type == 's')
		ft_strncat(*(data->str), data->buf, data->buf_len);
	else if (data->type == 'n')
	{
		if (data->buf_len >= data->str_n)
			ft_strncat(*(data->str), data->buf, data->str_n - 1);
		else
		{
			ft_strncat(*(data->str), data->buf, data->buf_len);
			data->str_n -= data->buf_len;
		}
	}
	else if (data->type == 'a')
		as_flush(data);
}

void			buffer(void *str, t_printf *data, size_t len)
{
	int			diff;
	long long	str_i;

	str_i = 0;
	while (BUF_SIZE - data->buf_len < (int)len)
	{
		diff = BUF_SIZE - data->buf_len;
		ft_memcpy(&(data->buf[data->buf_len]), (char *)(str + str_i), diff);
		len -= diff;
		str_i += diff;
		data->buf_len += diff;
		flush(data);
		data->buf_len = 0;
	}
	if (data->ret >= 0)
	{
		data->ret += len;
		ft_memcpy(&(data->buf[data->buf_len]), (char *)(str + str_i), len);
		data->buf_len += len;
	}
}

void			add_pad(t_printf *data, int n)
{
	if (data->pad <= 0)
		return ;
	if (CH_ZERO(data->flags))
		data->conv = 48;
	else
		data->conv = 32;
	if (!n && !CH_MINUS(data->flags))
		while ((data->pad)--)
			buffer((void *)&(data->conv), data, 1);
	else if (n && CH_MINUS(data->flags))
		while ((data->pad)--)
			buffer((void *)&(data->conv), data, 1);
}
