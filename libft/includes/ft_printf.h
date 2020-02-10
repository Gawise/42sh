/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guaubret <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 19:59:22 by guaubret          #+#    #+#             */
/*   Updated: 2019/08/21 19:08:26 by guaubret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <limits.h>
# include <errno.h>
# include <inttypes.h>
# include "libft.h"

# define F_SHARP		(1 << 0)
# define F_ZERO			(1 << 1)
# define F_MINUS		(1 << 2)
# define F_SPACE		(1 << 3)
# define F_PLUS			(1 << 4)
# define F_WWILD		(1 << 5)
# define F_PWILD		(1 << 6)
# define F_H			(1 << 7)
# define F_HH			(1 << 8)
# define F_L			(1 << 9)
# define F_LL			(1 << 10)
# define F_UPX			(1 << 11)
# define F_PCONV		(1 << 12)
# define F_Z			(1 << 13)
# define F_J			(1 << 14)
# define F_PGIVEN		(1 << 15)
# define BUF_SIZE		100

# define CH_SHARP(x)	(x & F_SHARP)
# define CH_ZERO(x)		(x & F_ZERO)
# define CH_MINUS(x)	(x & F_MINUS)
# define CH_SPACE(x)	(x & F_SPACE)
# define CH_PLUS(x)		(x & F_PLUS)
# define CH_WWILD(x)	(x & F_WWILD)
# define CH_PWILD(x)	(x & F_PWILD)
# define CH_H(x)		(x & F_H)
# define CH_HH(x)		(x & F_HH)
# define CH_L(x)		(x & F_L)
# define CH_LL(x)		(x & F_LL)
# define CH_UPX(x)		(x & F_UPX)
# define CH_PCONV(x)	(x & F_PCONV)
# define CH_Z(x)		(x & F_Z)
# define CH_J(x)		(x & F_J)
# define CH_PGIVEN(x)	(x & F_PGIVEN)

# define C_RED			"\033[0;31m"
# define C_BRED			"\033[1;31m"
# define C_GREEN		"\033[0;32m"
# define C_BGREEN		"\033[1;32m"
# define C_YELLOW		"\033[0;33m"
# define C_BYELLOW		"\033[1;33m"
# define C_BLUE			"\033[0;34m"
# define C_BBLUE		"\033[1;34m"
# define C_MAGENTA		"\033[0;35m"
# define C_BMAGENTA		"\033[1;35m"
# define C_CYAN			"\033[0;36m"
# define C_BCYAN		"\033[1;36m"
# define C_EOC			"\033[0m"

typedef struct		s_printf
{
	char			*fmt;
	char			*err_ptr;
	int				fd;
	char			**str;
	int				str_n;
	char			type;
	va_list			ap;
	char			buf[BUF_SIZE];
	int				buf_len;
	short			flags;
	short			f_index;
	int				width;
	int				prec;
	unsigned		conv;
	int				printed;
	int				pad;
	unsigned int	base;
	int				ret;
	void			*disp;
	int				disp_set;
	int				as;
}					t_printf;

void				read_arg(t_printf *data);
void				read_flags(t_printf *data);
void				read_width(t_printf *data);
void				read_prec(t_printf *data);
void				read_length_mod(t_printf *data);
void				read_conv_spec(t_printf *data);

void				flush(t_printf *data);
void				buffer(void *str, t_printf *data, size_t len);
void				add_pad(t_printf *data, int n);
int					init_type(t_printf *data, int fd, char **str, int str_n);
void				xtoa_base(uintmax_t val, int base,
		char s[21], t_printf *data);
void				init_dispatcher(t_printf *data,
		void (*disp[11])(t_printf *));
void				init_data(t_printf *data, const char *format, int as);

int					ft_printf(const char *format, ...);
int					ft_dprintf(int fd, const char *format, ...);
int					ft_sprintf(char *str, const char *format, ...);
int					ft_snprintf(char *str, size_t size,
		const char *format, ...);
int					ft_asprintf(char **ret, const char *format, ...);

void				conv_float(t_printf *data);
void				conv_int(t_printf *data);
void				conv_uint(t_printf *data);
void				conv_char(t_printf *data);
void				conv_str(t_printf *data);
void				conv_ptr(t_printf *data);
void				conv_color(t_printf *data);
void				conv_esc(t_printf *data);

#endif
