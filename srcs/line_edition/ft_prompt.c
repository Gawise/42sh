/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_prompt.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ambelghi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/02 17:01:55 by ambelghi          #+#    #+#             */
/*   Updated: 2020/02/13 18:45:13 by ambelghi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <termios.h>
#include <term.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include "line_edition.h"
#include <time.h>
#include <stdio.h>
#include "struct.h"

void	read_input(void)
{
	int		len;
	int		stop;
	char	*buf;
	int		ret;
	t_cs_line	*cs;

	stop = 0;
	len = 0;
	cs = cs_master(NULL, 0);
	while (stop >= 0)
	{
		cs->history->data = cs->input;
		ioctl(cs->tty, FIONREAD, &len);
		if (cs->sig_int == 0 && len <= 0)
			continue ;
		if (cs->sig_int == 1 || !(buf = ft_strnew(len + 1)))
			break ;
		if ((ret = read(cs->tty, buf, len)) != len
			|| (buf[0] == 4 && buf[1] == '\0'))
			stop = -1;
		stop = (stop >= 0 ? check_keys(buf) : stop);
		ft_strdel(&buf);
	}
}

void	ft_clear(int del_prompt)
{
	t_point			col;
	t_cs_line			*cs;
	struct winsize	ws;

	if ((cs = cs_master(NULL, 0)))
	{
		col.x = cs->min_col;
		col.y = cs->min_row;
		ioctl(cs->tty, TIOCGWINSZ, &ws);
		tputs(tgoto(tgetstr("cm", NULL), cs->min_col, cs->min_row),
				1, &my_putchar);
		while (col.y < ws.ws_row)
		{
			if (col.y++ == cs->min_row)
			{
				if (del_prompt == 1)
					tputs(tgetstr("ce", NULL), 1, &my_putchar);
				tputs(tgoto(tgetstr("cm", NULL), 0, cs->min_row + 1),
                1, &my_putchar);
			}
			else if (col.y - 1 != cs->min_row && col.y - 1 < ws.ws_row)
				tputs(tgetstr("dl", NULL), ws.ws_col, &my_putchar);
		}
		tputs(tgoto(tgetstr("cm", NULL), cs->min_col, cs->min_row),
				1, &my_putchar);
	}
}

char	*ft_prompt(char *prompt, t_dlist **lst)
{
	char		*ret;
	t_cs_line	*cs;

	ret = NULL;
	cs = NULL;
	if (term_init(1, prompt) == 1 && (cs = cs_master(NULL, 0)))
	{
		cs->sig_int = 0;
		get_cs_line_position(&cs->min_col, &cs->min_row);
		cs->history = ft_dlstnew(cs->input, 1);
		ft_dlstaddtail(lst, cs->history);
		read_input();
		init_signals();
		term_init(0, NULL);
		ft_putstr_fd("\n", cs->tty);
		ret = cs->input;
		if (cs->sig_int == 0 && ret && ret[0])
			cs->history->data = ft_strdup(ret);
		else if (cs->sig_int == 0)
		{
		//	while (cs && cs->history && cs->history->next)
		//		cs->history = cs->history->next;
			ft_dlstdelone(&cs->history);
			ret = NULL;
		}
	}
	return (((cs && cs->sig_int) || !ret ? ft_strnew(0) : ret));
}
