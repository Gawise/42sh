/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmerieux <hmerieux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/12 18:45:10 by hmerieux          #+#    #+#             */
/*   Updated: 2020/01/12 18:45:12 by hmerieux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "libft.h"

void		ft_convert_to_binary(void *buf, const char *convert, va_list ap)
{
	t_buf		*buff;
	size_t		size;
	long long	nb;
	char		*s;
	char		*tmp;

	(void)convert;
	buff = buf;
	nb = va_arg(ap, long long);
	s = ft_itoa_base(nb, 2);
	tmp = s;
	size = ft_strlen(s);
	if (size + buff->j > 4050)
		ft_printreset(buf);
	while (size-- > 0)
		buff->buf[buff->j++] = *s++;
	ft_strdel(&tmp);
}

static void	ft_print_bit(t_buf *buf, char *mem, int len)
{
	int		i;

	while (len--)
	{
		i = 9;
		while (--i > -1)
			buf->buf[buf->j++] = (1 & (*(char *)mem >> i) ? '1' : '0');
		buf->buf[buf->j++] = ' ';
		mem++;
	}
	buf->j--;
}

void		ft_print_memory(void *buf, const char *convert, va_list ap)
{
	char	*mem;
	int		size;
	int		zero;
	t_buf	*buff;

	buff = buf;
	zero = 0;
	size = 1;
	mem = va_arg(ap, char *);
	if (*convert != 'B')
		size = ft_atoi_convert(&zero, convert);
	if ((size * 8) + buff->j > 4050)
		ft_printreset(buf);
	ft_print_bit(buf, mem, size);
}

void		ft_convert_n(void *buf, const char *convert, va_list ap)
{
	unsigned long long	*nb;
	t_buf				*buff;

	(void)convert;
	buff = buf;
	nb = va_arg(ap, unsigned long long *);
	*nb = buff->nb + buff->j;
	nb = NULL;
}
