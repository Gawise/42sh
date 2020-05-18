/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ambelghi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/11 20:32:03 by ambelghi          #+#    #+#             */
/*   Updated: 2020/03/11 16:37:48 by ambelghi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_printf.h"
#include "sh.h"
#include "get_next_line.h"
#include "line_edition.h"
#include "struct.h"
#include <stdlib.h>

int ft_strcheck(char *s, char *oc)
{
    int i;

    if (s && oc)
    {
        i = 0;
        while (s[i])
            if (!ft_strchr(oc, s[i++]) && s[i - 1] != '\n')
                return (0);
        return (1);
    }
    return (0);
}

void	update_history(t_dlist *hs)
{
	int			fd;
	int			len;
	char		*path;
//	char		*tmp;
	t_cs_line	*cs;

	cs = cs_master(NULL, 0);
	if (hs && cs && cs->history)
	{
		ft_asprintf(&path, "%s/.%s_history", getenv("HOME"), PROJECT);
	/*	path = ft_strjoin(2, getenv("HOME"), "/");
		tmp = path;
		path = ft_strjoin(2, path, ".42sh_history");
		ft_strdel(&tmp); */
		len = ft_strlen(cs->input);
		cs->history->data = (void *)cs->old_history;
		while (hs && hs->next)
			hs = hs->next;
		if (hs && cs && path && cs->input && cs->input[0] && cs->input[0]
			!= '\n' && (fd = open(path, O_CREAT | O_APPEND | O_WRONLY, 0666)))
		{
			if (!(hs->prev && hs->prev->data && (cs->input[len - 1] = '\0')
				== 0 && ft_strcmp(cs->input, (char *)hs->prev->data) == 0))
			{
				ft_strdel((char **)&hs->data);
				hs->data = (void *)ft_strdup(cs->input);
				cs->input[len - 1] = '\n';
				ft_putstr_fd(cs->input, fd);
			}
			else
				ft_dlstdelone(&hs);
			ft_strdel(&cs->input);
			close(fd);
		}
		ft_strdel(&path);
	}
}

t_dlist	*get_history(void)
{
	t_dlist	*hs;
	char	*line;
//	char	*tmp;
	int		fd;

	if ((hs = ft_dlstnew(NULL, 0)))
	{
		ft_asprintf(&line, "%s/.%s_history", getenv("HOME"), PROJECT);
	/*	line = ft_strjoin(2, getenv("HOME"), "/");
		tmp = line;
		line = ft_strjoin(2, line, ".42sh_history");
		ft_strdel(&tmp); */
		if ((fd = open(line, O_RDONLY)) > 0)
		{
			ft_strdel(&line);
			while (get_next_line(fd, &line) > 0)
				ft_dlstaddtail(&hs, ft_dlstnew(line, 1));
			close(fd);
		}
		else
			ft_strdel(&line);
	}
	return (hs);
}

void	history_up(t_cs_line *cs)
{
	if (cs)
	{
		if (cs->history->prev && cs->history->prev->data)
		{
			ft_clear(1);
			cs->clipb = (t_point){-1, -1};
			cs->history = cs->history->prev;
			ft_strdel(&cs->old_history);
			cs->old_history = ft_strdup((char *)cs->history->data);
			cs->input = (char *)cs->history->data;
			cs->line_col = ft_strlen(cs->input);
			cs->scroll = 0;
			set_scroll(cs);
			print_cmdline(cs);
		}
	}
}

void	history_down(t_cs_line *cs)
{
	if (cs)
	{
		if (cs->history->next && cs->history->next->data)
		{
			ft_clear(1);
			cs->clipb = (t_point){-1, -1};
			cs->history = cs->history->next;
			ft_strdel(&cs->old_history);
			cs->old_history = ft_strdup((char *)cs->history->data);
			cs->input = (char *)cs->history->data;
			cs->line_col = ft_strlen(cs->input);
			cs->scroll = 0;
			print_cmdline(cs);
		}
	}
}

t_dlist	*init_history(void)
{
	t_dlist *hist;

	hist = get_history();
	return (hist);
}
