/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert_octal.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmerieux <hmerieux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/12 18:46:10 by hmerieux          #+#    #+#             */
/*   Updated: 2020/01/12 18:46:11 by hmerieux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "libft.h"

static void		ft_feed_buf(char c, int len, t_buf *buf)
{
	if (buf->j + len > 4050)
		ft_printreset(buf);
	while (len-- > 0)
		buf->buf[buf->j++] = c;
}

static int		ft_adjustflag_octal(t_flags *flags, int len)
{
	if (flags->diez == 1)
		len += 1;
	flags->preci = (flags->preci - len >= 0) ? (flags->preci - len) : 0;
	flags->large = (flags->large - flags->preci - len >= 0)
		? (flags->large - flags->preci - len) : 0;
	if (flags->zero == 1 && flags->left == 1)
		flags->zero = 0;
	if (flags->precinull)
		flags->zero = 0;
	if (flags->zero)
	{
		flags->zero = flags->large;
		if (flags->zero == 1)
			flags->zero = 0;
		flags->large = 0;
	}
	return (flags->left = (flags->large > 0) ? flags->left : 0);
}

static void		ft_left_feedoctal_buf(t_buf *buff, t_flags *flags, char *nb)
{
	int		i;

	i = 0;
	if (flags->preci)
	{
		if (nb[i] == '-')
			buff->buf[buff->j++] = nb[i++];
		ft_feed_buf('0', flags->preci, buff);
	}
	if (flags->diez == 1)
		buff->buf[buff->j++] = '0';
	while (nb[i])
	{
		if (buff->j >= 4000)
			ft_printreset(buff);
		buff->buf[buff->j++] = nb[i++];
	}
	if (flags->large)
		ft_feed_buf(' ', flags->large, buff);
}

static void		ft_right_feedoctal_buf(t_buf *buff, t_flags *flags, char *nb)
{
	int		i;

	i = 0;
	if (flags->large)
		ft_feed_buf(' ', flags->large, buff);
	if (flags->zero)
	{
		if (nb[i] == '-')
			buff->buf[buff->j++] = nb[i++];
		ft_feed_buf('0', flags->zero, buff);
	}
	if (flags->preci)
	{
		if (nb[i] == '-')
			buff->buf[buff->j++] = nb[i++];
		ft_feed_buf('0', flags->preci, buff);
	}
	if (flags->diez == 1)
		buff->buf[buff->j++] = '0';
	while (nb[i])
	{
		if (buff->j >= 4000)
			ft_printreset(buff);
		buff->buf[buff->j++] = nb[i++];
	}
}

void			ft_convert_octal(void *buf, const char *convert, va_list ap)
{
	t_flags flags;
	t_buf	*buff;
	char	*nb;

	buff = buf;
	ft_init_structflags(&flags);
	ft_grap_flags(&flags, convert, ap, -1);
	nb = ft_grap_unb(ap, convert, 8);
	if (ft_strcmp(nb, "0") == 0)
	{
		if (flags.diez || flags.precinull)
			nb[0] = '\0';
	}
	if (ft_adjustflag_octal(&flags, ft_strlen(nb)))
		ft_left_feedoctal_buf(buff, &flags, nb);
	else
		ft_right_feedoctal_buf(buff, &flags, nb);
	ft_strdel(&nb);
}
