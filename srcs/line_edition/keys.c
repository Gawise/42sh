/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ambelghi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/23 14:45:32 by ambelghi          #+#    #+#             */
/*   Updated: 2020/02/16 17:41:30 by ambelghi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "line_edition.h"
#include "struct.h"
#include <sys/stat.h>
#include <sys/types.h>
#include "libft.h"
#include <sys/ioctl.h>

t_point	trim_input(t_cs_line *cs)
{
	t_point z;
	t_point	i;
	int		line;

	z = (t_point){0, 0};
	if (cs && cs->input && (z.y = (int)ft_strlen(cs->input)) >= 0)
	{
		i = (t_point){0, 0};
		line = 0;
		while (cs->input[i.x])
		{
			i.y++;
			if (((line == 0 && i.y + cs->min_col >= cs->screen.x - 1)
				|| i.y == cs->screen.x) && ++line)
				i.y = 0;
			if (cs->scroll > 0 && line > cs->scroll && !z.x && (z.x = i.x + 1))
				ft_putstr_fd("\e[0;31m\e[47m\e[1m\e[4m\e[7m^\e[0m\n", cs->tty);
			if (line - cs->scroll + cs->min_row + 1 > cs->screen.y
				&& (z.y = i.x - 1) >= 0)
				break ;
			i.x++;
		}
	}
	return (z);
}

void	print_prompt(t_cs_line *cs)
{
	if (cs)
	{
		tputs(tgoto(tgetstr("cm", NULL), 0, cs->min_row),
				1, &my_putchar);
		if (cs->screen.x <= (int)ft_strlen(cs->prompt))
			ft_putstr_fd("$ ", cs->tty);
		else if (cs->screen.x == 1)
			ft_putstr_fd("$", cs->tty);
		else
			ft_putstr_fd(cs->prompt, cs->tty);
	}
}

void	cmdline_printer(t_cs_line *cs, t_point start, t_point end)
{
	char	oc;

	if (cs)
	{
		oc = cs->input[start.y];
		cs->input[start.y] = '\0';
		ft_putstr_fd(&cs->input[start.x], cs->tty);
		cs->input[start.y] = oc;
		if (cs->clipb.x != -1 && cs->clipb.y != -1 && cs->clipb.x != cs->clipb.y)
			tputs(tgetstr("mr", NULL), 1, &my_putchar);
		oc = cs->input[end.y];
		cs->input[end.y] = '\0';
		ft_putstr_fd(&cs->input[start.y], cs->tty);
		cs->input[end.y] = oc;
		if (cs->clipb.x != -1 && cs->clipb.y != -1 && cs->clipb.x != cs->clipb.y)
			tputs(tgetstr("me", NULL), 1, &my_putchar);
		oc = cs->input[end.x];
		cs->input[end.x] = '\0';
		ft_putstr_fd(&cs->input[end.y], cs->tty);
		cs->input[end.x] = oc;
	}
}

void	print_cmdline(t_cs_line *cs)
{
	t_point		z;
	t_point		start;
	t_point		end;

	ft_clear(1);
	if (cs && cs->input && cs->line_col >= 0)
	{
		print_prompt(cs);
		z = trim_input(cs);
		start.x = z.x;
		end.x = z.y;
		if (end.x < (int)ft_strlen(cs->input))
			end.x++;
		start.y = (cs->clipb.x >= start.x && cs->clipb.x <= end.x)
			? cs->clipb.x : start.x;
		end.y = (cs->clipb.y >= start.x && cs->clipb.y <= end.x)
			? cs->clipb.y : end.x;
		cmdline_printer(cs, start, end);
		move_cs(&cs);
	}
}

void	join_input(t_cs_line *cs, char *input)
{
	char        *tmp;
	char        *insert;
	char		oc;

	if (cs && input)
	{
		if ((int)ft_strlen(cs->input) == cs->line_col && (tmp = cs->input))
		{
			cs->input = ft_strjoin(2, cs->input, input);
			ft_strdel(&tmp);
		}
		else if (cs->input && cs->input[0])
		{
			oc = cs->input[cs->line_col];
			cs->input[cs->line_col] = '\0';
			tmp = cs->input;
			cs->input = ft_strjoin(2, tmp, input);
			tmp[cs->line_col] = oc;
			insert = cs->input;
			cs->input = ft_strjoin(2, insert, &tmp[cs->line_col]);
			ft_strdel(&insert);
			ft_strdel(&tmp);
		}
		ft_utoa(&cs->input);
	}
}

void	line_master(t_cs_line *cs, char *input)
{
	if (input && cs)
	{
		join_input(cs, input);
		cs->line_col += (int)ft_strlen(input);
		cs->max_scroll = (int)ft_strlen(cs->input) / cs->screen.x
			- (cs->screen.y - cs->min_row);
		set_scroll(cs);
		cs->history->data = cs->input;
		cs->clipb = (t_point){-1, -1};
	}
}

t_point	cs_pos(t_cs_line *cs)
{
	t_point	pos;
	t_point	i;
	int		cr;

	i = (t_point){0, 0};
	pos = (t_point){0, 0};
	cr = 0;
	if (cs && cs->input && cs->input[0])
	{
		cs->min_col = (int)ft_strlen(cs->prompt);
		if (cs->screen.x <= (int)ft_strlen(cs->prompt))
			cs->min_col = (cs->screen.x > 3 ? 2 : 0);
		while (i.x < cs->line_col || (i.x == cs->line_col && cs->scroll > 0
					&& cs->screen.x > 2))
		{
			if ((cs->input[i.x] == '\n' || i.y == cs->screen.x
				|| (cr == 0 && i.y + cs->min_col >= cs->screen.x)) && ++cr)
				i.y = 0;
			i = (t_point){i.x + 1, i.y + 1};
		}
		i.y = (i.y > cs->line_col ? cs->line_col : i.y);
		pos.x = i.y + (cr == 0 ? cs->min_col : 0);
		pos.y = cr - cs->scroll + cs->min_row + (cs->screen.x > 2 ? 0 : 1);
	}
	return (pos);
}

int		get_line(t_cs_line *cs)
{
	t_point	i;
	t_point	cr;

	if (cs->input && cs->input[0])
	{
		cr = (t_point){0, 0};
		i = (t_point){cs->line_col, 0};
		while (cs->input[i.x] && cr.x == 0 && i.y++ >= 0)
		{
			if ((cs->input[i.x] == '\n' || i.y == cs->screen.x) && ++cr.x)
				i.y = 0;
			i.x++;
		}
		i = (t_point){0, 0};
		cr.y = cr.x;
		while (i.x <= cs->line_col && i.y++ >= 0)
		{
			if (((cr.y == cr.x && i.y + cs->min_col >= cs->screen.x)
				|| cs->input[i.x] == '\n' || i.y == cs->screen.x) && ++cr.x)
				i.y = 0;
			i.x++;
		}
	}
	return (cr.x);
}

void	arrow_up(t_cs_line *cs)
{
	if (cs)
	{
		if (cs->line_col > cs->screen.x)
			cs->line_col -= cs->screen.x;
		else if (cs->line_col > cs->screen.x - cs->min_col)
			cs->line_col -= cs->screen.x - cs->min_col;
		cs->cr = get_line(cs);
		if (cs->cr + cs->min_row - cs->scroll - 1 <= cs->min_row && cs->scroll)
		{
			cs->scroll = cs->cr - (cs->screen.y - cs->min_row
					+ (cs->screen.x > 2 ? 0 : 1) + (cs->scroll > 0 ? 1 : 0) - 1);
			print_cmdline(cs);
		}
		if (cs->scroll < 0)
		{
			cs->scroll = 0;
			print_cmdline(cs);
		}
		move_cs(&cs);
	}
}

void	arrow_down(t_cs_line *cs)
{
	int	scroll;

	if (cs)
	{
		scroll = cs->scroll;
		if (cs->line_col + cs->screen.x <= (int)ft_strlen(cs->input))
			cs->line_col += cs->screen.x;
		else
			cs->line_col = (int)ft_strlen(cs->input);
		cs->cr = get_line(cs);
		if (cs->cr - cs->scroll + cs->min_row >= cs->screen.y)
		{
			cs->scroll = cs->scroll = cs->cr - (cs->screen.y - (cs->min_row
						+ (cs->screen.x > 2 ? 0 : 1) + (cs->scroll > 0 ? 1 : 0)) - 1);
			print_cmdline(cs);
		}
		if (cs->scroll < 0)
			cs->scroll = 0;
		move_cs(&cs);	
	}
}

void	arrow_right(t_cs_line *cs)
{

	if (cs)
	{
		if (cs->line_col < (int)ft_strlen(cs->input))
			cs->line_col += 1;
		if (cs->col == cs->screen.x && cs->row >= cs->screen.y - 1)
		{
			cs->scroll += 1;
			print_cmdline(cs);
		}
		if (cs->scroll < 0)
			cs->scroll = 0;
		move_cs(&cs);
	}	
}

void	arrow_left(t_cs_line *cs)
{

	if (cs)
	{
		if (cs->line_col > 0)
			cs->line_col -= 1;
		if (cs->col == 1 && cs->scroll && cs->row == cs->min_row
				+ (cs->screen.x > 1 ? 0 : 1) + (cs->scroll > 0 ? 1 : 0))
		{
			cs->scroll -= 1;
			print_cmdline(cs);
		}
		if (cs->scroll < 0)
			cs->scroll = 0;
		move_cs(&cs);
	}
}

void	home_key(t_cs_line *cs)
{
	int	cr;

	if (cs)
	{
		cs->line_col = 0;
		cr = get_line(cs);
		cs->scroll = cr - (cs->screen.y - cs->min_row - 1);
		if (cs->scroll < 0)
			cs->scroll = 0;
		print_cmdline(cs);
		move_cs(&cs);
	}
}

void	end_key(t_cs_line *cs)
{
	int	cr;

	if (cs)
	{
		cs->line_col = (int)ft_strlen(cs->input);
		cr = get_line(cs);
		cs->scroll = cr - (cs->screen.y - cs->min_row - 1);
		if (cs->scroll < 0)
			cs->scroll = 0;
		print_cmdline(cs);
		move_cs(&cs);
	}
}
