/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ambelghi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/13 14:27:55 by ambelghi          #+#    #+#             */
/*   Updated: 2020/03/06 21:43:42 by hmerieux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <term.h>
#include <termios.h>
#include <sys/ioctl.h>
#include "line_edition.h"
#include <unistd.h>
#include <stdlib.h>
#include "libft.h"
#include "struct.h"

void	size_handler(int sig)
{
	t_cs_line			*cs;

	if (sig == SIGWINCH && (cs = cs_master(NULL, 0)))
	{
		cs->min_row = 0;
		cs->min_col = 0;
		ft_clear(1);
		cs_set();
		int cr = get_line(cs);
		if (cs->screen.x > (int)ft_strlen(cs->prompt) + (cs->scroll ? 1 : 0))
            cs->min_col = (int)ft_strlen(cs->prompt);
        else
            cs->min_col = 0;//(cs->scroll ? 0 : cs->screen.x - 1);
		cs->min_row = 0;
		cr = get_line(cs);
		cs->cr = cr;
		cs->scroll = cs->cr - (cs->screen.y - cs->min_row - 1);
		if (cs->scroll < 0)
			cs->scroll = 0;
		print_cmdline(cs);
	}
}

void	pause_handler(int sig)
{
	t_cs_line	*cs;

	if ((sig == SIGTSTP || sig == SIGTTIN || sig == SIGTTOU || sig == SIGSTOP)
			&& (cs = cs_master(NULL, 0)))
	{
		signal(SIGTSTP, SIG_DFL);
		signal(SIGTTIN, SIG_DFL);
		signal(SIGTTOU, SIG_DFL);
		signal(SIGSTOP, SIG_DFL);
		ioctl(cs->tty, TIOCSTI, "\x1A");
	}
}

void	sig_handler(int sig)
{
	t_cs_line *cs;

	if (sig >= 0 && (cs = cs_master(NULL, 0)))
	{
		term_init(0, NULL);
		ft_clear(1);
		ft_putstr_fd(cs->input, cs->tty);
		ft_strdel(&cs->input);
		if (cs->history)
			cs->history->data = NULL;
		cs->sig_int = 1;
	}
}
