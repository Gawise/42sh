/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert_hexal.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmerieux <hmerieux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/12 18:46:02 by hmerieux          #+#    #+#             */
/*   Updated: 2020/01/12 18:46:05 by hmerieux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "libft.h"

static void		ft_feed_buf(char c, int len, t_buf *buf)
{
	if (buf->j >= 4050 || len >= 50)
		ft_printreset(buf);
	while (len-- > 0)
		buf->buf[buf->j++] = c;
}

static int		ft_adjustflag_hexal(t_flags *flags, int len)
{
	if (flags->diez == 1)
		flags->large -= 2;
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

static void		ft_left_feedhexal_buf(t_buf *buf, t_flags *flags, char *nb)
{
	int		i;

	i = 0;
	if (flags->diez == 1)
	{
		buf->buf[buf->j++] = '0';
		buf->buf[buf->j++] = 'X';
	}
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

static void		ft_right_feedhexal_buf(t_buf *buf, t_flags *flags, char *nb)
{
	int		i;

	i = 0;
	if (flags->large)
		ft_feed_buf(' ', flags->large, buf);
	if (flags->diez == 1)
	{
		buf->buf[buf->j++] = '0';
		buf->buf[buf->j++] = 'X';
	}
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

void			ft_convert_hexal(void *buf, const char *convert, va_list ap)
{
	t_flags		flags;
	char		*nb;
	int			tmp;

	tmp = ((t_buf *)(buf))->j - 1;
	ft_init_structflags(&flags);
	ft_grap_flags(&flags, convert, ap, -1);
	nb = ft_grap_unb(ap, convert, 16);
	if (ft_strcmp(nb, "0") == 0)
	{
		if (flags.precinull && !flags.preci)
			nb[0] = '\0';
		flags.diez = 0;
	}
	if (ft_adjustflag_hexal(&flags, ft_strlen(nb)))
		ft_left_feedhexal_buf(buf, &flags, nb);
	else
		ft_right_feedhexal_buf(buf, &flags, nb);
	if (ft_strstr(convert, "x"))
		while (tmp++ < ((t_buf *)(buf))->j)
			if (((t_buf *)(buf))->buf[tmp] >= 'A'
					&& ((t_buf *)(buf))->buf[tmp] <= 'Z')
				((t_buf *)(buf))->buf[tmp] += 32;
	ft_strdel(&nb);
}
