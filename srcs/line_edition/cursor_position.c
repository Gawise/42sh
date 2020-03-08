/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cursor_position.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ambelghi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/08 16:20:12 by ambelghi          #+#    #+#             */
/*   Updated: 2020/03/08 17:11:31 by ambelghi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "line_edition.h"
#include "struct.h"
#include "libft.h"

t_point	cs_pos(t_cs_line *cs)
{
	t_point pos;
	t_point i;
	int		cr;
	int		tmp_mincol;

	i = (t_point){0, 0};
	pos = (t_point){0, 0};
	cr = 0;
	if (cs && cs->input && cs->input[0])
	{
		tmp_mincol = cs->min_col;
		cs->min_col += (int)ft_strlen(cs->prompt);
		if (cs->scroll || cs->screen.x <= (int)ft_strlen(cs->prompt))
			cs->min_col = 0;
		while (i.x < cs->line_col || (i.x == cs->line_col && cs->scroll > 0
					&& cs->screen.x > 0))
		{
			if ((cs->input[i.x] == '\n' || i.y == cs->screen.x || (cr == 0
				&& !cs->scroll && cs->screen.x > (int)ft_strlen(cs->prompt)
				&& i.y + cs->min_col >= cs->screen.x)) && ++cr)
				i.y = 0;
			i = (t_point){i.x + 1, i.y + 1};
		}
		i.y = (i.y > cs->line_col ? cs->line_col : i.y);
		pos.x = i.y + (cr == 0 && cs->screen.x > (int)ft_strlen(cs->prompt)
				? cs->min_col : 0) - (cs->scroll ? 1 : 0);
		pos.y = cr - cs->scroll + cs->min_row + (cs->screen.x
				<= (int)ft_strlen(cs->prompt) ? 1 : 0);
		cs->min_col = tmp_mincol;
	}
	return (pos);
}

int		get_line(t_cs_line *cs)
{
	t_point i;
	t_point cr;

	if (cs->input && cs->input[0])
	{
		cr = (t_point){0, 0};
		i = (t_point){cs->line_col, 0};
		while (cs->input[i.x] && cr.x == 0 && i.y++ >= 0)
		{
			if ((i.y == cs->screen.x || (i.x + cs->min_col
				+ (int)ft_strlen(cs->prompt) == cs->screen.x)) && ++cr.x)
				i.y = 0;
			i.x++;
		}
		i = (t_point){0, 0};
		cr.y = cr.x;
		while (i.x <= cs->line_col && i.y++ >= 0)
		{
			if ((i.y == cs->screen.x || (i.x + cs->min_col
				+ (int)ft_strlen(cs->prompt) == cs->screen.x)) && ++cr.x)
				i.y = 0;
			i.x++;
		}
	}
	return (cr.x);
}
