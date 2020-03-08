/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_prompt.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ambelghi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/08 17:15:04 by ambelghi          #+#    #+#             */
/*   Updated: 2020/03/08 17:16:45 by ambelghi         ###   ########.fr       */
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
	int			len;
	int			stop;
	char		*buf;
	int			ret;
	t_cs_line	*cs;

	stop = 0;
	len = 0;
	cs = cs_master(NULL, 0);
	if (cs->history)
		cs->history->data = cs->input;
	while (stop >= 0)
	{
		ioctl(cs->tty, FIONREAD, &len);
		if (cs->sig_int == 0 && len <= 0)
			continue ;
		if (cs->sig_int == 1 || !(buf = ft_strnew(len + 1)))
			break ;
		if (cs->history)
			cs->history->data = cs->input;
		if ((ret = read(cs->tty, buf, len)) != len)
			stop = -1;
		stop = (stop >= 0 ? check_keys(buf) : stop);
		ft_strdel(&buf);
	}
}

void	ft_clear(int del_prompt)
{
	t_point			col;
	t_cs_line		*cs;
	struct winsize	ws;
	int				col_prompt;

	if ((cs = cs_master(NULL, 0)))
	{
		col.x = cs->min_col;
		col.y = cs->min_row;
		ioctl(cs->tty, TIOCGWINSZ, &ws);
		col_prompt = (int)ft_strlen(cs->prompt);
		col_prompt -= (col_prompt > cs->screen.x ? cs->screen.x : 0);
		tputs(tgoto(tgetstr("cm", NULL), cs->min_col + col_prompt, cs->min_row),
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
		tputs(tgoto(tgetstr("cm", NULL), cs->min_col + col_prompt, cs->min_row),
				1, &my_putchar);
	}
}

char	*ft_prompt(char *prompt)
{
	char		*ret;
	t_cs_line	*cs;
	t_dlist		*hs;

	ret = NULL;
	cs = NULL;
	if (term_init(1, prompt) == 1 && (cs = cs_master(NULL, 0)))
	{
		ft_clear(0);
		hs = get_history();
		cs->sig_int = 0;
		cs->history = ft_dlstnew(cs->input, 1);
		ft_dlstaddtail(&hs, cs->history);
		read_input();
		term_init(0, NULL);
		ft_putstr_fd("\n", cs->tty);
		ret = ft_strdup(cs->input);
		if (!cs->sig_int)
		{
			update_history(hs);
			ft_dlstdel(&cs->history);
		}
	}
	return ((cs->sig_eof) ? ft_strnew(0) : ret);
}
