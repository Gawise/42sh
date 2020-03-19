/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ambelghi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/13 14:31:14 by ambelghi          #+#    #+#             */
/*   Updated: 2020/03/08 17:35:37 by ambelghi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "line_edition.h"
#include "libft.h"
#include "struct.h"

int			my_putchar(int c)
{
	ft_putchar_fd((char)c, ttyslot());
	return (1);
}

static char	**split_pos(const char *s)
{
	int		i;
	char	**ret;

	i = 0;
	while (*(s + i) && (*(s + i) == 27 || *(s + i) == '['))
		i++;
	ret = ft_strsplit((s + i), ";");
	return (ret);
}

void		ft_utoa(char **str)
{
	char	*s;
	int		i;

	if (str && (s = *str))
	{
		i = 0;
		while (s[i])
		{
			if ((unsigned char)s[i] > 127)
			{
				if ((unsigned char)s[i] == 130 || (unsigned char)s[i] == 136 ||
					(unsigned char)s[i] == 137 || (unsigned char)s[i] == 138)
					s[i] = 'e';
				else if ((unsigned char)s[i] == 132 || (unsigned char)s[i]
						== 133 || (unsigned char)s[i] == 134
						|| (unsigned char)s[i] == 160)
					s[i] = 'a';
				else if ((unsigned char)s[i] == 147 || (unsigned char)s[i]
						== 148 || (unsigned char)s[i] == 149
						|| (unsigned char)s[i] == 162)
					s[i] = 'o';
				else if ((unsigned char)s[i] == 129 || (unsigned char)s[i]
						== 150 || (unsigned char)s[i] == 151
						|| (unsigned char)s[i] == 163)
					s[i] = 'u';
				else
					s[i] = '@';
			}
			i++;
		}
	}
}

void		get_cs_line_position(int *col, int *row)
{
	char		buf[16];
	char		**n;
	int			fd;
	t_cs_line	*cs;

	ft_bzero(&buf, 16);
	cs = cs_master(NULL, 0);
	fd = cs->tty;
	if (read(fd, &buf, 0) == -1)
		return ;
	write(fd, "\033[6n", ft_strlen("\033[6n"));
	read(fd, &buf, 16);
	if (!(n = split_pos(buf)))
	{
		*col = 0;
		*row = 0;
		return ;
	}
	*row = ft_atoi(n[0]) - 1;
	*col = ft_atoi(n[1]) - 1;
	ft_strdel(&n[0]);
	ft_strdel(&n[1]);
	free(n);
}

void		putchar_n(char c, int n)
{
	t_cs_line	*cs;

	cs = cs_master(NULL, 0);
	while (n-- > 0)
		write(cs->tty, &c, 1);
}
