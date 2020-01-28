/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dprintf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmerieux <hmerieux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/23 17:21:24 by hmerieux          #+#    #+#             */
/*   Updated: 2020/01/23 17:21:33 by hmerieux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "libft.h"
#include <stdarg.h>
#include <unistd.h>
#define BUFF_SIZE 4096

int		ft_checkdelimite(char c)
{
	char	*delimite;
	char	*ofr;
	int		i;
	int		j;

	i = -1;
	j = -1;
	delimite = "idcsuoxX%fpbBnDUO";
	ofr = "#-+ 0123456789hlL.*";
	while (delimite[++i] != c && delimite[i])
		;
	if (i < 17)
		return (i);
	while (ofr[++j] && ofr[j] != c)
		;
	if (j == 19)
		return (19);
	return (-1);
}

void	ft_gearing(t_buf *buf, const char *format, va_list ap)
{
	char	convert[128];
	int		c;
	int		t;
	int		tmp;

	c = -1;
	t = 0;
	tmp = buf->i;
	ft_bzero(convert, 128);
	while (format[buf->i])
	{
		convert[++c] = format[++buf->i];
		if (((t = ft_checkdelimite(format[buf->i])) > -1))
			break ;
	}
	convert[c + 1] = '\0';
	if (t >= 0 && t < 19)
		buf->tab[t].ftpr(buf, convert, ap);
	else
	{
		buf->buf[buf->j++] = '%';
		buf->i = tmp;
	}
}

void	ft_printreset(t_buf *buf)
{
	write(buf->fd, buf->buf, buf->j);
	buf->nb += buf->j;
	buf->j = 0;
}

int		ft_dprintf(int fd, const char *format, ...)
{
	t_buf	buf;
	va_list ap;

	ft_init_structbuf(&buf, fd);
	va_start(ap, format);
	while (format[buf.i])
	{
		while (format[buf.i] != '%' && format[buf.i])
		{
			if (buf.j + 1 == BUFF_SIZE)
				ft_printreset(&buf);
			buf.buf[buf.j++] = format[buf.i++];
		}
		if (format[buf.i] == '%')
		{
			if (format[buf.i + 1] != '%')
				ft_gearing(&buf, format, ap);
			else
				buf.buf[buf.j++] = format[buf.i++];
			buf.i++;
		}
	}
	ft_printreset(&buf);
	return (buf.nb);
}
