/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert_string.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmerieux <hmerieux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/12 18:46:25 by hmerieux          #+#    #+#             */
/*   Updated: 2020/01/12 18:46:26 by hmerieux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "libft.h"
#include <stdlib.h>

static void		ft_feed_buf(char c, int len, t_buf *buf)
{
	if (buf->j + len > 4050)
		ft_printreset(buf);
	while (len-- > 0)
		buf->buf[buf->j++] = c;
}

static int		ft_adjustflag_string(t_flags *flags, char **s, int len)
{
	if (flags->preci > 0 && flags->preci < len)
		len = flags->preci;
	else if (flags->precinull > 0 && flags->preci == 0)
	{
		*s = NULL;
		len = 0;
	}
	flags->large = (flags->large - len >= 0) ? (flags->large - len) : 0;
	if (flags->zero)
	{
		flags->zero = flags->large;
		if (flags->zero == 1)
			flags->zero = 0;
		flags->large = 0;
	}
	return (flags->left = (flags->large > 0) ? flags->left : 0);
}

static void		ft_left_feedstring_buf(t_buf *buf, t_flags *flags, char *s)
{
	int		i;
	int		len;

	i = 0;
	len = (flags->preci > 0) ? flags->preci : ft_strlen(s);
	if (buf->j + len >= 4050)
		ft_printreset(buf);
	while (s && s[i] && len--)
		buf->buf[buf->j++] = s[i++];
	if (flags->large)
		ft_feed_buf(' ', flags->large, buf);
}

static void		ft_right_feedstring_buf(t_buf *buf, t_flags *flags, char *s)
{
	int		i;
	int		j;
	char	c;

	j = 0;
	i = 0;
	c = (flags->zero > 0) ? '0' : ' ';
	while (flags->zero - j > 0 || flags->large - j > 0)
	{
		if (buf->j >= 4050)
			ft_printreset(buf);
		buf->buf[buf->j++] = c;
		j++;
	}
	j = (flags->preci > 0) ? flags->preci : ft_strlen(s);
	while (s && s[i] && j--)
	{
		if (buf->j >= 4050)
			ft_printreset(buf);
		buf->buf[buf->j++] = s[i++];
	}
}

void			ft_convert_string(void *buf, const char *convert, va_list ap)
{
	t_flags	flags;
	t_buf	*buff;
	char	*s;

	buff = buf;
	ft_init_structflags(&flags);
	ft_grap_flags(&flags, convert, ap, -1);
	s = va_arg(ap, char *);
	if (s == NULL)
		s = "(null)";
	if (ft_adjustflag_string(&flags, &s, ft_strlen(s)))
		ft_left_feedstring_buf(buff, &flags, s);
	else
		ft_right_feedstring_buf(buff, &flags, s);
}
