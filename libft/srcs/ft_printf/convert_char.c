/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert_char.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmerieux <hmerieux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/12 18:45:20 by hmerieux          #+#    #+#             */
/*   Updated: 2020/01/12 18:45:22 by hmerieux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int		ft_adjustflag_char(t_flags *flags)
{
	if (flags->zero && flags->large > 0)
	{
		flags->zero = flags->large;
		flags->large = 0;
	}
	return ((flags->left = (flags->large > 0) ? flags->left : 0));
}

static void		ft_left_feedchar_buf(t_buf *buf, t_flags *flags, char c)
{
	buf->buf[buf->j++] = c;
	if (buf->j + flags->large >= 4050)
		ft_printreset(buf);
	while (flags->large-- > 1)
		buf->buf[buf->j++] = ' ';
}

static void		ft_right_feedchar_buf(t_buf *buf, t_flags *flags, char c)
{
	if (buf->j + flags->zero + flags->large >= 4050)
		ft_printreset(buf);
	while (flags->zero-- > 1)
		buf->buf[buf->j++] = '0';
	while (flags->large-- > 1)
		buf->buf[buf->j++] = ' ';
	buf->buf[buf->j++] = c;
}

void			ft_convert_char(void *buf, const char *convert, va_list ap)
{
	t_flags flags;
	char	c;

	ft_init_structflags(&flags);
	ft_grap_flags(&flags, convert, ap, -1);
	c = va_arg(ap, int);
	if (ft_adjustflag_char(&flags))
		ft_left_feedchar_buf(buf, &flags, c);
	else
		ft_right_feedchar_buf(buf, &flags, c);
}

void			ft_convert_percent(void *buf, const char *convert, va_list ap)
{
	t_flags flags;
	char	c;

	c = '%';
	(void)ap;
	ft_init_structflags(&flags);
	ft_grap_flags(&flags, convert, ap, -1);
	if (ft_adjustflag_char(&flags))
		ft_left_feedchar_buf(buf, &flags, c);
	else
		ft_right_feedchar_buf(buf, &flags, c);
}
