/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   Bt
: ambelghi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/13 16:27:29 by ambelghi          #+#    #+#             */
/*   Updated: 2020/02/11 15:40:45 by ambelghi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "line_edition.h"
#include <sys/stat.h>
#include <sys/types.h>
#include "libft.h"
#include <signal.h>
#include <sys/ioctl.h>

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
		arrow_down(cs);
	if (ft_strcmp(caps, "\e[1;2A") == 0 && (ret = 1))
		maj_arrow_up(cs);
	if (ft_strcmp(caps, "\e[1;2B") == 0 && (ret = 1))
		maj_arrow_down(cs);
	if (ft_strcmp(caps, "\e[A") == 0 && (ret = 1))
		arrow_up(cs);
	if (ft_strcmp(caps, "\e[H") == 0 && (ret = 1))
		home_key(cs);
	if (ft_strcmp(caps, "\e[F") == 0 && (ret = 1))
		end_key(cs);
	if (ft_strcmp(caps, "\n") == 0)
        ret = -1;
	if (ft_strcmp(caps, "\033[6n") == 0 || caps[0] == '[')
		ret = -1;
	if (caps[0] != 127 && ret == 0 && caps[0] != '\033' && (ret = 1))
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
