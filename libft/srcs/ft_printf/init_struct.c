/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_struct.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmerieux <hmerieux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/12 18:46:41 by hmerieux          #+#    #+#             */
/*   Updated: 2020/01/23 17:32:07 by hmerieux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "libft.h"

void	ft_create2_convert(t_buf *buf)
{
	buf->tab[12].c = 'B';
	buf->tab[12].ftpr = &ft_print_memory;
	buf->tab[13].c = 'n';
	buf->tab[13].ftpr = &ft_convert_n;
	buf->tab[14].c = 'D';
	buf->tab[14].ftpr = &ft_convert_integer;
	buf->tab[15].c = 'U';
	buf->tab[15].ftpr = &ft_convert_integer;
	buf->tab[16].c = 'O';
	buf->tab[16].ftpr = &ft_convert_octal;
}

void	ft_create_convert(t_buf *buf)
{
	buf->tab[0].c = 'i';
	buf->tab[0].ftpr = &ft_convert_integer;
	buf->tab[1].c = 'd';
	buf->tab[1].ftpr = &ft_convert_integer;
	buf->tab[2].c = 'c';
	buf->tab[2].ftpr = &ft_convert_char;
	buf->tab[3].c = 's';
	buf->tab[3].ftpr = &ft_convert_string;
	buf->tab[4].c = 'u';
	buf->tab[4].ftpr = &ft_convert_integer;
	buf->tab[5].c = 'o';
	buf->tab[5].ftpr = &ft_convert_octal;
	buf->tab[6].c = 'x';
	buf->tab[6].ftpr = &ft_convert_hexal;
	buf->tab[7].c = 'X';
	buf->tab[7].ftpr = &ft_convert_hexal;
	buf->tab[8].c = '%';
	buf->tab[8].ftpr = &ft_convert_percent;
	buf->tab[9].c = 'f';
	buf->tab[9].ftpr = &ft_convert_float;
	buf->tab[10].c = 'p';
	buf->tab[10].ftpr = &ft_convert_pointeur;
	buf->tab[11].c = 'b';
	buf->tab[11].ftpr = &ft_convert_to_binary;
	ft_create2_convert(buf);
}

void	ft_init_structbuf(t_buf *buf, int fd)
{
	buf->fd = fd;
	buf->i = 0;
	buf->j = 0;
	buf->nb = 0;
	ft_create_convert(buf);
	ft_bzero(buf->buf, 4096);
}

void	ft_init_structflags(t_flags *flags)
{
	flags->large = 0;
	flags->left = 0;
	flags->zero = 0;
	flags->preci = 0;
	flags->precinull = 0;
	flags->plus = 0;
	flags->space = 0;
	flags->diez = 0;
	flags->wc = 0;
}
