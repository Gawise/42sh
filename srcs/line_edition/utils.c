/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ambelghi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/13 14:31:14 by ambelghi          #+#    #+#             */
/*   Updated: 2020/02/11 17:08:05 by ambelghi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "line_edition.h"
#include "libft.h"

int		my_putchar(int c)
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
				if ((unsigned char)s[i] == 130 || (unsigned char)s[i] == 136
					|| (unsigned char)s[i] == 137 || (unsigned char)s[i] == 138)
					s[i] = 'e';
				else if ((unsigned char)s[i] == 132 || (unsigned char)s[i] == 133
						|| (unsigned char) s[i] == 134 || (unsigned char)s[i] == 160)
					s[i] = 'a';
				else if ((unsigned char)s[i] == 147 || (unsigned char)s[i] == 148
						|| (unsigned char)s[i] == 149 || (unsigned char)s[i] == 162)
					s[i] = 'o';
				else if ((unsigned char)s[i] == 129 || (unsigned char)s[i] == 150
						|| (unsigned char)s[i] == 151 || (unsigned char)s[i] == 163)
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
	char	buf[16];
	char	**n;

	ft_bzero(&buf, 16);
	t_cs_line *cs = cs_master(NULL, 0);
	int fd = cs->tty;
	write(fd, "\033[6n", ft_strlen("\033[6n"));
	read(fd, &buf, 16);
	if (!(n = split_pos(buf)))
	{
		*col = 0;
		*row = 0;
		return ;
	}
//	read(ttyslot(), &buf, 0);
	*row = ft_atoi(n[0]) - 1;
	*col = ft_atoi(n[1]) - 1;
	ft_strdel(&n[0]);
	ft_strdel(&n[1]);
	free(n);

}

int		get_maxlen(t_line_lst *list)
{
	t_line_lst	*tmp;
	int				max_len;
	int				ncol;

	max_len = 0;
	ncol = 0;
	if ((tmp = list))
	{
		while ((tmp = tmp->next))
		{
	//		if ((ncol = ft_strlen(tmp->name)) > max_len)
	//			max_len = ncol;
		}
	}
	return (max_len);
}

void	putchar_n(char c, int n)
{
	t_cs_line	*cs;

	cs = cs_master(NULL, 0);
	while (n-- > 0)
		write(cs->tty, &c, 1);
}
