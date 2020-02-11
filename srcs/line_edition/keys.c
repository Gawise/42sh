/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ambelghi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/23 14:45:32 by ambelghi          #+#    #+#             */
/*   Updated: 2020/02/11 17:38:30 by ambelghi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "line_edition.h"
#include <sys/stat.h>
#include <sys/types.h>
#include "libft.h"
#include <sys/ioctl.h>

t_point	trim_input(t_cs_line *cs)
{
	t_point z;
	int		line;
	int		i;
	int		i3;
	int		ew;
	char	*more;
	char	*tmp;

	z.x = 0;
	z.y = (int)ft_strlen(cs->input);
	if (cs)
	{
		i3 = 0;
		ew = 0;
		i = 0;
		line = 0;
		tmp = cs->input;
		while (tmp[i])
		{
			i3++;
			if (i3 == cs->screen.x || (line == 0 && i3
						+ cs->min_col >= cs->screen.x - 1))
			{
				i3 = 0;
				line++;
			}
			if ((cs->scroll > 0 && line > cs->scroll && ew == 0))
			{
				more = "^";
				ft_putstr_fd("\e[0;31m\e[47m", cs->tty);
				ft_putstr_fd("\e[1m\e[4m\e[7m", cs->tty);
				ft_putstr_fd(more, cs->tty);
				ft_putstr_fd("\e[0m", cs->tty);
				ft_putstr_fd("\n", cs->tty);
				ew = 1;
				z.x = i + 1;
			}
			if (line - cs->scroll + cs->min_row + 1 > cs->screen.y)
			{
				z.y = i - 1;
				break ;
			}
			i++;
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

void	print_cmdline(t_cs_line *cs)
{
	int			z;
	int			start;
	int			end;
	char		oc;

	ft_clear(1);
	if (cs && cs->input && cs->line_col >= 0)
	{
		print_prompt(cs);
		z = 0;
		t_point z = trim_input(cs);
		start = z.x;
		end = z.y;
		oc = cs->input[end + 1];
		cs->input[end + 1] = '\0';
		ft_putstr_fd(&cs->input[start], cs->tty);
		cs->input[end + 1] = oc;
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
		if ((int)ft_strlen(cs->input) == cs->line_col)
		{
			tmp = cs->input;
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
	int	i;
	int	row_prompt;
	int	scroll_add;

	if (input && cs)
	{
		join_input(cs, input);
		cs->line_col += (int)ft_strlen(input);
		cs->max_scroll = (int)ft_strlen(cs->input) / cs->screen.x
			- (cs->screen.y - cs->min_row);
		i = 0;
		cs->cr = get_line(cs);
		row_prompt = PROMPT_SIZE / cs->screen.x;
		if (cs->cr + cs->min_row >= cs->screen.y && cs->min_row > row_prompt)
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

t_point	cs_pos(t_cs_line *cs)
{
	t_point	ew;
	int i = 0;
	int cr = 0;
	int i3 = 0;

	ew.x = 0;
	ew.y = 0;
	if (cs && cs->input && cs->input[0])
	{
		if (cs->screen.x > (int)ft_strlen(cs->prompt))
			cs->min_col = (int)ft_strlen(cs->prompt);
		else
			cs->min_col = (cs->screen.x > 3 ? 2 : 0);
		while (i < cs->line_col || (i == cs->line_col && cs->scroll > 0
					&& cs->screen.x > 2))
		{
			if (cs->input[i] == '\n' || i3 == cs->screen.x
					|| (cr == 0 && i3 + cs->min_col >= cs->screen.x))
			{
				i3 = 0;
				cr++;
			}
			i3++;
			i++;
		}
		if (i3 > cs->line_col)
			i3 = cs->line_col;
		ew.x = i3 + (cr == 0 ? cs->min_col : 0);
		ew.y = cr - cs->scroll + cs->min_row + (cs->screen.x > 2 ? 0 : 1);
	}
	return (ew);
}

int		get_line(t_cs_line *cs)
{
	int i = 0;
	int cr = 0;
	int i3 = 0;
	int i4 = cs->line_col;
	int	cr2 = 0;

	if (cs->input && cs->input[0])
	{
		while (cs->input[i4] && cr == 0)
		{
			i3++;
			if (cs->input[i4] == '\n' || i3 == cs->screen.x)
			{
				i3 = 0;
				cr++;
			}
			i4++;
		}
		i3 = 0;
		cr2 = cr;
		while (i <= cs->line_col)
		{
			i3++;
			if (cs->input[i] == '\n' || i3 == cs->screen.x
					|| (cr2 == cr && i3 + cs->min_col >= cs->screen.x))
			{
				i3 = 0;
				cr++;
			}
			i++;
		}
	}
	return (cr);
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

void	maj_arrow_down(t_cs_line *cs)
{
	t_line_lst  *tmp;
	tmp = NULL;
	cs = NULL;
	/*
	   if (cs && (tmp = line_getlst(cs->list, cs->line + 1)))
	   {
	   if (cs->line < cs->nb_lines && tmp->next)
	   {
	   cs->line += 1;
	   if (cs->line_col % cs->screen.x > tmp->next->len)
	   cs->line_col = tmp->next->len;
	   else
	   cs->line_col = cs->line_col % cs->screen.x;
	   }
	   move_cs(&cs);
	   }*/
}

void	maj_arrow_up(t_cs_line *cs)
{
	t_line_lst  *tmp;
	tmp = NULL;
	cs = NULL;
	/*
	   if (cs && (tmp = line_getlst(cs->list, cs->line + 1)))
	   {
	   if (cs->line > 0 && tmp->prev)
	   {
	   if (cs->line_col % cs->screen.x > tmp->prev->len)
	   cs->line_col = tmp->prev->len;
	   else
	   cs->line_col = cs->line_col % cs->screen.x;
	   cs->line -= 1;
	   }
	   move_cs(&cs);
	   }
	 */
}
