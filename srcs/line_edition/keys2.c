/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ambelghi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/16 12:58:26 by ambelghi          #+#    #+#             */
/*   Updated: 2020/02/17 20:48:02 by ambelghi         ###   ########.fr       */
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
		if (cs->cr + cs->min_row >= cs->screen.y && cs->min_row > (cs->scroll ? 1 : 0))
		{
			scroll_add = cs->cr + cs->min_row - cs->screen.y + 1;
			while (cs->min_row - scroll_add < 0)
				scroll_add--;
			cs->min_row -= scroll_add;
			tputs(tgoto(tgetstr("SF", NULL), 0, scroll_add), 1, &my_putchar);
		}
		if (cs->cr - cs->scroll + cs->min_row >= cs->screen.y)
			cs->scroll = cs->cr - (cs->screen.y - cs->min_row - 1);
		if (cs->scroll < 0)
			cs->scroll = 0;
	}
}

int		back_space(t_cs_line *cs)
{
	char			*oc;
	char			*tmp2;

	if (cs)
	{
		if (cs->line_col > 0)
		{
			if((tmp2 = cs->input))
			{
				if (cs->line_col > 0)
				{
					oc = &tmp2[cs->line_col - 1];
					*oc = '\0';
					if (tmp2[cs->line_col] != '\0')
						cs->input = ft_strjoin(2, tmp2, &tmp2[cs->line_col]);
					else
						cs->input = ft_strdup(tmp2);
					ft_strdel(&tmp2);
				}
				int row_prompt;
				row_prompt = (PROMPT_SIZE + (cs->scroll > 0 ? 5 : 0)) / cs->screen.x
					+ (((PROMPT_SIZE + (cs->scroll > 0 ? 5 : 0)) % cs->screen.x) > 0
							? 1 : 0);
				if (cs->line_col > 0)
					cs->line_col -= 1;
				if (cs->col == 1 && cs->scroll && cs->row == cs->min_row
						+ (cs->screen.x > 1 ? 0 : 1) + (cs->scroll > 0 ? 1 : 0))
					cs->scroll -= 2;
				if (cs->scroll < 0)
					cs->scroll = 0;
				print_cmdline(cs);
			}
		}
		return (1);
	}
	return (-1);
}

int		check_keys(char *caps)
{
	int				ret;
	struct winsize	w_term;
	t_cs_line			*cs;

	ret = 0;
	ioctl(STDIN_FILENO, TIOCGWINSZ, &w_term);
	cs = cs_master(NULL, 0);
	if (ft_strcmp(caps, "\e[D") == 0 && (ret = 1))
		arrow_left(cs);
	if (ft_strcmp(caps, "\e[C") == 0 && (ret = 1))
		arrow_right(cs);
	if ((ft_strcmp(caps, "\e[B") == 0 || ft_strcmp(caps, "\x09") == 0)
			&& (ret = 1))
		history_down(cs);
	if (ft_strcmp(caps, "\e[1;2A") == 0 && (ret = 1))
		arrow_up(cs);
	if (ft_strcmp(caps, "\e[1;2B") == 0 && (ret = 1))
		arrow_down(cs);
	if (ft_strcmp(caps, "\e[1;2C") == 0 && (ret = 1))
		mv_word_right(cs);
	if (ft_strcmp(caps, "\e[1;2D") == 0 && (ret = 1))
		mv_word_left(cs);
	if (ft_strcmp(caps, "\e[A") == 0 && (ret = 1))
		history_up(cs);
	if ((ft_strcmp(caps, "\e[H") == 0 || caps[0] == 1) && (ret = 1))
		home_key(cs);
	if ((ft_strcmp(caps, "\e[F") == 0 || caps[0] == 5) && (ret = 1))
		end_key(cs);
	if (ft_strcmp(caps, "\e[1;6C") == 0 && (ret = 1))
		clip_arrow_right(cs);
	if (ft_strcmp(caps, "\e[1;6D") == 0 && (ret = 1))
        	clip_arrow_left(cs);
	if (ft_strcmp(caps, "\e[1;6A") == 0 && (ret = 1))
	        clip_arrow_up(cs);
	if (ft_strcmp(caps, "\e[1;6B") == 0 && (ret = 1))
        	clip_arrow_down(cs);
	if (caps[0] == (char)11)
		copy_clip(cs);
	if (caps[0] == (char)16)
		paste_clip(cs);
	if (caps[0] == (char)12)
        	cut_clip(cs);
	if (ft_strcmp(caps, "\n") == 0 || ft_strcmp(caps, "\eEOF") == 0)
	{
		ret = -1;
		if (ft_strcmp(caps, "\n") == 0)
			line_master(cs, caps);
	}
	if (ft_strcmp(caps, "\033[6n") == 0 || caps[0] == '[')
		ret = -1;
	if (caps[0] != 127 && ret == 0 && caps[0] != '\033' && caps[0] >= 32
		&& (ret = 1))
	{
		line_master(cs, caps);
		if (ft_strchr(caps, '\n'))
			ret = -1;
		print_cmdline(cs);
	}
	if (caps[0] == 127)
	{
		ret = back_space(cs);
		print_cmdline(cs);
	}
	if ((int)ft_strlen(caps) == 1 && caps[0] == '\033' && (ret = -1) < 0)
		ft_clear(0);
	return (ret);
}
