/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert_pointeur.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmerieux <hmerieux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/12 18:46:16 by hmerieux          #+#    #+#             */
/*   Updated: 2020/01/12 18:46:19 by hmerieux         ###   ########.fr       */
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

static int		ft_adjustflag_pointeur(t_flags *flags, int len)
{
	flags->preci = (flags->preci - len >= 0) ? (flags->preci - len) : 0;
	flags->large = (flags->large - flags->preci - (len + 2) >= 0)
		? (flags->large - flags->preci - (len + 2)) : 0;
	if (flags->left == 1)
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

static void		ft_left_feedpointeur_buf(t_buf *buf, t_flags *flags, char *nb)
{
	int		i;

	i = 0;
	buf->buf[buf->j++] = '0';
	buf->buf[buf->j++] = 'x';
	if (flags->preci)
		ft_feed_buf('0', flags->preci, buf);
	while (nb && nb[i])
	{
		if (buf->j >= 4050)
			ft_printreset(buf);
		buf->buf[buf->j++] = nb[i++];
	}
	if (flags->large)
		ft_feed_buf(' ', flags->large, buf);
}

static void		ft_right_feedpointeur_buf(t_buf *buf, t_flags *flags, char *nb)
{
	int		i;

	i = 0;
	if (flags->large)
		ft_feed_buf(' ', flags->large, buf);
	buf->buf[buf->j++] = '0';
	buf->buf[buf->j++] = 'x';
	if (flags->zero)
		ft_feed_buf('0', flags->zero, buf);
	if (flags->preci)
		ft_feed_buf('0', flags->preci, buf);
	while (nb && nb[i])
	{
		if (buf->j >= 4050)
			ft_printreset(buf);
		buf->buf[buf->j++] = nb[i++];
	}
}

void			ft_convert_pointeur(void *buf, const char *convert, va_list ap)
{
	t_flags		flags;
	char		*nb;

	ft_init_structflags(&flags);
	ft_grap_flags(&flags, convert, ap, -1);
	nb = ft_grap_pnb(ap, flags);
	if (ft_adjustflag_pointeur(&flags, ft_strlen(nb)))
		ft_left_feedpointeur_buf(buf, &flags, nb);
	else
		ft_right_feedpointeur_buf(buf, &flags, nb);
	ft_strdel(&nb);
}
