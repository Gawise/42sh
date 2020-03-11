/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   other_keys.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ambelghi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/08 16:41:44 by ambelghi          #+#    #+#             */
/*   Updated: 2020/03/11 15:59:37 by ambelghi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "struct.h"
#include "line_edition.h"

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
		if (cs->sig_int)
			ft_putstr_fd("^C", cs->tty);
		move_cs(&cs);
	}
}

int		ctrl_d(t_cs_line *cs)
{
	if (cs)
	{
		if (ft_strlen(cs->input) > 0)
			return (revback_space(cs));
		else
		{
			ft_strdel(&cs->input);
			if (cs->history)
				cs->history->data = NULL;
			cs->sig_eof = 1;
		}
	}
	return (-1);
}
