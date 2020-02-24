/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clipboard.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ambelghi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/15 19:22:49 by ambelghi          #+#    #+#             */
/*   Updated: 2020/02/17 14:36:20 by ambelghi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "get_next_line.h"
#include "line_edition.h"
#include "struct.h"
#include <stdlib.h>

void    clip_arrow_right(t_cs_line *cs)
{

	if (cs && cs->input && cs->input[0])
	{
		if (cs->line_col < (int)ft_strlen(cs->input))
			cs->line_col += 1;
		if (cs->clipb.x == cs->clipb.y || cs->line_col < cs->clipb.y)
			cs->clipb.x = cs->line_col - 1;
		cs->clipb.y = cs->line_col;
		if (cs->col == cs->screen.x && cs->row >= cs->screen.y - 1)
			cs->scroll += 1;
		if (cs->scroll < 0)
			cs->scroll = 0;
		print_cmdline(cs);
	}
}

void    clip_arrow_left(t_cs_line *cs)
{

	if (cs && cs->input && cs->input[0])
	{
		if (cs->line_col > 0)
			cs->line_col -= 1;
		if (cs->clipb.x == cs->clipb.y || cs->line_col > cs->clipb.x)
			cs->clipb.y = cs->line_col + 1;
		cs->clipb.x = cs->line_col;
		if (cs->col == 1 && cs->scroll && cs->row == cs->min_row
				+ (cs->screen.x > 1 ? 0 : 1) + (cs->scroll > 0 ? 1 : 0))
			cs->scroll -= 1;
		if (cs->scroll < 0)
			cs->scroll = 0;
		print_cmdline(cs);
	}
}

void    clip_arrow_up(t_cs_line *cs)
{

	if (cs && cs->input && cs->input[0])
	{
		if (cs->line_col > cs->screen.x)
		{
			cs->line_col -= cs->screen.x;
			if (cs->clipb.x == cs->clipb.y || cs->line_col > cs->clipb.x)
				cs->clipb.y = cs->line_col + cs->screen.x;
		}
		else if (cs->line_col > cs->screen.x - cs->min_col)
		{
			cs->line_col -= cs->screen.x - cs->min_col;
			if (cs->clipb.x == cs->clipb.y || cs->line_col > cs->clipb.x)
				cs->clipb.y = cs->line_col + cs->screen.x - cs->min_col;
		}
		cs->clipb.x = cs->line_col;
		cs->cr = get_line(cs);
		if (cs->cr + cs->min_row - cs->scroll - 1 <= cs->min_row && cs->scroll)
		{
			cs->scroll = cs->cr - (cs->screen.y - cs->min_row
					+ (cs->screen.x > 2 ? 0 : 1) + (cs->scroll > 0 ? 1 : 0) - 1);
		}
		if (cs->scroll < 0)
			cs->scroll = 0;
		print_cmdline(cs);
	}
}

void    clip_arrow_down(t_cs_line *cs)
{
	int scroll;

	if (cs && cs->input && cs->input[0])
	{
		scroll = cs->scroll;
		if (cs->line_col + cs->screen.x <= (int)ft_strlen(cs->input))
		{
			cs->line_col += cs->screen.x;
			if (cs->clipb.x == cs->clipb.y || cs->line_col < cs->clipb.y)
				cs->clipb.x = cs->line_col - cs->screen.x;
		}
		else
			cs->line_col = (int)ft_strlen(cs->input);
		cs->clipb.y = cs->line_col;
		cs->cr = get_line(cs);
		if (cs->cr - cs->scroll + cs->min_row >= cs->screen.y)
		{
			cs->scroll = cs->scroll = cs->cr - (cs->screen.y - (cs->min_row
				+ (cs->screen.x > 2 ? 0 : 1) + (cs->scroll > 0 ? 1 : 0)) - 1);
		}
		if (cs->scroll < 0)
			cs->scroll = 0;
		print_cmdline(cs);
	}
}

void	copy_clip(t_cs_line *cs)
{
	if (cs)
	{
		if (cs->clipb.x > -1 && cs->clipb.y > -1 && cs->clipb.y - cs->clipb.x > 0)
		{
			ft_strdel(&cs->clipboard);
			cs->clipboard = ft_strsub(cs->input, cs->clipb.x, cs->clipb.y - cs->clipb.x);
		}
	}
}

void	paste_clip(t_cs_line *cs)
{
	if (cs && cs->clipboard)
	{
		line_master(cs, cs->clipboard);
		cs->clipb = (t_point){0, 0};
		print_cmdline(cs);
	}
}

void	cut_clip(t_cs_line *cs)
{
	char	*cut;
	char	*tmp;
	char	oc;

	copy_clip(cs);
	if (cs->clipb.x != -1 && cs->clipb.y != -1 && cs->clipb.y - cs->clipb.x > 0)
    {
		ft_clear(1);
		oc = cs->input[cs->clipb.x];
		cs->input[cs->clipb.x] = '\0';
		cut = ft_strdup(cs->input);
		cs->input[cs->clipb.x] = oc;
		tmp = cut;
		cs->line_col = cs->clipb.x;
		cut = ft_strjoin(2, cut, &cs->input[cs->clipb.y]);
		ft_strdel(&tmp);
		ft_strdel(&cs->input);
		cs->input = cut;
		cs->clipb = (t_point){-1, -1};
		print_cmdline(cs);
	}
}
