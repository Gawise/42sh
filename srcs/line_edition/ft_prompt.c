/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_prompt.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ambelghi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/08 17:15:04 by ambelghi          #+#    #+#             */
/*   Updated: 2020/03/11 17:23:43 by ambelghi         ###   ########.fr       */
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
#include "sh.h"
#include "get_next_line.h"

char	*read_nonint(t_cfg	*cfg)
{
	char		*line;
	static int	fd = -2;
	int			ret;

	if (fd == -2)
	{
		if (cfg && (fd = open(cfg->file, O_RDONLY)) < 0)
			ft_ex("Bad file descriptor\n");
		if (dup2(fd, 259) == -1)
			ft_ex(EX);
		close(fd);
		fd = 259;
	}
	line = ft_strnew(0);
	if (cfg)
	{
		ft_strdel(&line);
		if ((ret = get_next_line(fd, &line)) < 0)
			ft_ex("An error occured while reading file\n");
		else if (ret == 0)
			line = ft_strnew(0);
	}
	return (line);
}

void	read_input(void)
{
	int			len;
	int			stop;
	char		buf[9];
	t_cs_line	*cs;

	stop = 0;
	cs = cs_master(NULL, 0);
	if (cs->history)
		cs->history->data = cs->input;
	while (stop >= 0)
	{
		len = read(cs->tty, buf, READ_SIZE);
		if (len < 0)
            stop = -1;
		buf[len] = '\0';
		if (cs->history)
			cs->history->data = cs->input;
		stop = (stop >= 0 ? check_keys(buf) : stop);
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

char	*ft_prompt(char *prompt, char *color)
{
	char		*ret;
	t_cs_line	*cs;
	t_dlist		*hs;
	t_cfg		*cfg;

	ret = NULL;
	cs = NULL;
	if (!(cfg = cfg_shell()))
    	return (NULL);
	if (!cfg->interactive)
		return (read_nonint(cfg));
	if (term_init(1, prompt) == 1 && (cs = cs_master(NULL, 0)))
	{
		cs->prompt_color = color;
		print_prompt(cs);
		if (!(cfg = cfg_shell()))
			return (NULL);
		hs = cfg->history;
		cs->sig_int = 0;
		cs->history = ft_dlstnew(cs->input, 1);
		ft_dlstaddtail(&hs, cs->history);
		read_input();
		end_key(cs);
		tputs(tgoto(tgetstr("cm", NULL), 0, cs->row), 1, &my_putchar);
		term_init(0, NULL);
		if (cs->input && cs->input[0] && !ft_strcheck(cs->input, " \t")
		&& ft_strcmp(cs->input, "\n") != 0 && (ret = ft_strdup(cs->input)) >= 0)
			update_history(hs);
		else
			ft_dlstdelone(&cs->history);
		set_signal_ign();
		ft_strdel(&cs->clipboard);
	}
	return ((cs && cs->sig_eof) ? ft_strnew(0) : ret);
}
