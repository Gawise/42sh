/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert_digit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmerieux <hmerieux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/12 18:45:37 by hmerieux          #+#    #+#             */
/*   Updated: 2020/01/12 18:45:42 by hmerieux         ###   ########.fr       */
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

static int		ft_adjustflag_integer(t_flags *flags, int len, char *nb)
{
	if (nb[0] == '-')
	{
		flags->space = 0;
		flags->plus = 0;
		len -= 1;
		flags->large -= 1;
	}
	flags->preci = (flags->preci - len >= 0) ? (flags->preci - len) : 0;
	flags->large = (flags->large - flags->preci - len >= 0)
		? (flags->large - flags->preci - len) : 0;
	if (flags->space == 1 && flags->plus == 1)
		flags->space = 0;
	if (flags->zero == 1 && flags->left == 1)
		flags->zero = 0;
	if ((flags->space == 1 || flags->plus == 1) && flags->large > 0)
		flags->large -= 1;
	if (flags->precinull)
		flags->zero = 0;
	if (flags->zero)
	{
		flags->zero = flags->large;
		flags->large = 0;
	}
	return (flags->left = (flags->large > 0) ? flags->left : 0);
}

static void		ft_left_feedinteger_buf(t_buf *buf, t_flags *flags, char *nb)
{
	int		i;

	i = 0;
	if (flags->plus == 1)
		buf->buf[buf->j++] = '+';
	if (flags->space == 1)
		buf->buf[buf->j++] = ' ';
	if (flags->preci)
	{
		if (nb[i] == '-')
			buf->buf[buf->j++] = nb[i++];
		ft_feed_buf('0', flags->preci, buf);
	}
	while (nb[i])
	{
		if (buf->j >= 4050)
			ft_printreset(buf);
		buf->buf[buf->j++] = nb[i++];
	}
	if (flags->large)
		ft_feed_buf(' ', flags->large, buf);
}

static void		ft_right_feedinteger_buf(t_buf *buf, t_flags *flags, char *nb)
{
	int		i;

	i = 0;
	if (flags->large)
		ft_feed_buf(' ', flags->large, buf);
	if (flags->plus == 1)
		buf->buf[buf->j++] = '+';
	if (flags->space == 1)
		buf->buf[buf->j++] = ' ';
	if (nb[i] == '-')
		buf->buf[buf->j++] = nb[i++];
	if (flags->zero)
		ft_feed_buf('0', flags->zero, buf);
	if (flags->preci)
		ft_feed_buf('0', flags->preci, buf);
	while (nb[i])
	{
		if (buf->j >= 4050)
			ft_printreset(buf);
		buf->buf[buf->j++] = nb[i++];
	}
}

void			ft_convert_integer(void *buf, const char *convert, va_list ap)
{
	t_flags flags;
	char	*nb;

	ft_init_structflags(&flags);
	ft_grap_flags(&flags, convert, ap, -1);
	if (ft_strstr(convert, "u") || ft_strstr(convert, "U"))
		nb = ft_grap_unb(ap, convert, 10);
	else
		nb = ft_grap_nb(ap, convert);
	if (nb && nb[0] == '0' && flags.precinull)
		nb[0] = '\0';
	if (ft_adjustflag_integer(&flags, ft_strlen(nb), nb))
		ft_left_feedinteger_buf(buf, &flags, nb);
	else
		ft_right_feedinteger_buf(buf, &flags, nb);
	ft_strdel(&nb);
}
