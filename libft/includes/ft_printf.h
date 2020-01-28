/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmerieux <hmerieux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/12 18:41:41 by hmerieux          #+#    #+#             */
/*   Updated: 2020/01/23 17:53:16 by hmerieux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>

typedef struct			s_flags
{
	int					large;
	int					left;
	int					zero;
	int					preci;
	int					precinull;
	int					plus;
	int					space;
	int					diez;
	int					wc;
}						t_flags;

typedef struct			s_convert
{
	char				c;
	void				(*ftpr)(void *buf, const char *convert, va_list ap);
}						t_convert;

typedef struct			s_buf
{
	int					fd;
	int					i;
	int					j;
	unsigned long long	nb;
	t_convert			tab[17];
	char				buf[4096];
}						t_buf;

typedef struct			s_float
{
	int					s;
	char				*sipart;
	char				*sfpart;
	long double			ipart;
	long double			fpart;
}						t_float;

typedef struct			s_bitfield_flt
{
	unsigned long		m:64;
	unsigned int		e:15;
	unsigned int		s:1;
}						t_bt_flt;

/*
** Ft_printf
*/

int						ft_dprintf(int fd, const char *format, ...);

/*
** Conversion
*/

void					ft_create_convert(t_buf *buf);
void					ft_convert_char(void *buf,
							const char *convert, va_list ap);
void					ft_convert_string(void *buf,
							const char *convert, va_list ap);
void					ft_convert_integer(void *buf,
							const char *convert, va_list ap);
void					ft_convert_octal(void *buf,
							const char *convert, va_list ap);
void					ft_convert_hexal(void *buf,
							const char *convert, va_list ap);
void					ft_convert_percent(void *buf,
							const char *convert, va_list ap);
void					ft_convert_pointeur(void *buf,
							const char *convert, va_list ap);
void					ft_convert_float(void *buf,
							const char *convert, va_list ap);
void					ft_ldouble(t_float *flt, t_flags *flags);
void					ft_double(t_float *flt, t_flags *flags);

/*
** Tools
*/

int						ft_atoi_convert(int *i, const char *convert);
char					*ft_grap_nb(va_list ap, const char *convert);
char					*ft_grap_unb(va_list ap, const char *convert, int base);
char					*ft_grap_pnb(va_list ap, t_flags flags);
char					*ft_itoa_base(long long value, int base);
char					*ft_uitoa_base(unsigned long long value, int base);
void					ft_printreset(t_buf *buf);
void					ft_init_structbuf(t_buf *buf, int fd);
void					ft_init_structflags(t_flags *flags);
void					ft_grap_flags(t_flags *flags,
							const char *convert, va_list ap, int i);

/*
***************	Bonus %n %b %B %U %D *
*/

void					ft_convert_n(void *buf,
							const char *convert, va_list ap);
void					ft_print_memory(void *buf,
							const char *convert, va_list ap);
void					ft_convert_to_binary(void *buf,
							const char *convert, va_list ap);

#endif
