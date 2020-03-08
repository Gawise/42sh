/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ambelghi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/16 12:58:26 by ambelghi          #+#    #+#             */
/*   Updated: 2020/03/08 17:21:28 by ambelghi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "line_edition.h"
#include "struct.h"
#include <sys/stat.h>
#include <sys/types.h>
#include "libft.h"
#include <signal.h>
#include <sys/ioctl.h>
#include <stdio.h>

void	set_scroll(t_cs_line *cs)
{
	int	scroll_add;

	if (cs)
	{
		cs->cr = get_line(cs);
		if (cs->cr + cs->min_row >= cs->screen.y && cs->min_row > (cs->screen.x
			<= (int)ft_strlen(cs->prompt) || cs->scroll ? 1 : 0))
		{
			scroll_add = cs->cr + cs->min_row - cs->screen.y + 1;
			while (cs->min_row - scroll_add < 0)
				scroll_add--;
			cs->min_row -= scroll_add;
			tputs(tgoto(tgetstr("SF", NULL), 0, scroll_add), 1, &my_putchar);
		}
		if (cs->cr - cs->scroll + cs->min_row + (cs->screen.x <= (int)
			ft_strlen(cs->prompt) || cs->scroll == 0 ? 1 : 0) >= cs->screen.y)
			cs->scroll = cs->cr - (cs->screen.y - cs->min_row - 1);
		if (cs->scroll < 0)
			cs->scroll = 0;
	}
}

int		check_keys(char *caps)
{
	int				ret;
	struct winsize	w_term;
	t_cs_line		*cs;

	ret = 0;
	ioctl(STDIN_FILENO, TIOCGWINSZ, &w_term);
	cs = cs_master(NULL, 0);
	if (ft_strcmp(caps, "\e[D") == 0 && (ret = 1))
		arrow_left(cs);
	else if (ft_strcmp(caps, "\e[C") == 0 && (ret = 1))
		arrow_right(cs);
	else if ((ft_strcmp(caps, "\e[B") == 0 || ft_strcmp(caps, "\x09") == 0)
			&& (ret = 1))
		history_down(cs);
	else if (ft_strcmp(caps, "\e[1;2A") == 0 && (ret = 1))
		arrow_up(cs);
	else if (ft_strcmp(caps, "\e[1;2B") == 0 && (ret = 1))
		arrow_down(cs);
	else if (ft_strcmp(caps, "\e[1;2C") == 0 && (ret = 1))
		mv_word_right(cs);
	else if (ft_strcmp(caps, "\e[1;2D") == 0 && (ret = 1))
		mv_word_left(cs);
	else if (ft_strcmp(caps, "\e[A") == 0 && (ret = 1))
		history_up(cs);
	else if ((ft_strcmp(caps, "\e[H") == 0 || caps[0] == 1) && (ret = 1))
		home_key(cs);
	else if ((ft_strcmp(caps, "\e[F") == 0 || caps[0] == 5) && (ret = 1))
		end_key(cs);
	else if (ft_strcmp(caps, "\e[1;6C") == 0 && (ret = 1))
		clip_arrow_right(cs);
	else if (ft_strcmp(caps, "\e[1;6D") == 0 && (ret = 1))
		clip_arrow_left(cs);
	else if (ft_strcmp(caps, "\e[1;6A") == 0 && (ret = 1))
		clip_arrow_up(cs);
	else if (ft_strcmp(caps, "\e[1;6B") == 0 && (ret = 1))
		clip_arrow_down(cs);
	else if (caps[0] == (char)11)
		copy_clip(cs);
	else if (caps[0] == (char)16)
		paste_clip(cs);
	else if (caps[0] == (char)12)
		cut_clip(cs);
	else if (ft_strcmp(caps, "\n") == 0)
	{
		ret = -1;
		if (ft_strcmp(caps, "\n") == 0)
			line_master(cs, caps);
	}
	else if (ft_strcmp(caps, "\033[6n") == 0)
		ret = -1;
	else if (caps[0] != 127 && ret == 0 && caps[0] != '\033' && caps[0] >= 32
			&& (ret = 1))
	{
		line_master(cs, caps);
		if (ft_strchr(caps, '\n'))
			ret = -1;
		print_cmdline(cs);
	}
	else if (caps[0] == 127)
	{
		ret = back_space(cs);
		print_cmdline(cs);
	}
	else if (ft_strcmp(caps, "\e[3~") == 0)
	{
		ret = revback_space(cs);
		print_cmdline(cs);
	}
	else if (caps[0] == 4)
	{
		ret = ctrl_d(cs);
	}
	else if ((int)ft_strlen(caps) == 1 && caps[0] == '\033'
			&& (ret = -1) < 0)
		ft_clear(0);
	return (ret);
}
