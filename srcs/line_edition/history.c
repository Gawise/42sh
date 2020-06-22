#include "libft.h"
#include "ft_printf.h"
#include "sh.h"
#include "get_next_line.h"
#include "var.h"
#include "line_edition.h"
#include "struct.h"
#include <stdlib.h>

int			ft_strcheck(char *s, char *oc)
{
	int	i;

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

static char	*get_home(void)
{
	t_cfg	*cfg;
	char	*home;

	if ((cfg = cfg_shell()))
		if ((home = find_var_value(cfg->env, "HOME")))
			return (home);
	return (".");
}

static void	history_updater(t_cs_line *cs, t_dlist *hs)
{
	int		len;
	t_cfg	*cfg;
	char	*nb;

	len = ft_strlen(cs->input);
	if (!(hs->prev && hs->prev->data && (cs->input[len - 1] = '\0')
		== 0 && ft_strcmp(cs->input, (char *)hs->prev->data) == 0))
	{
		ft_strdel((char **)&hs->data);
		hs->data = (void *)ft_strdup(cs->input);
		cs->input[len - 1] = '\n';
		cfg = cfg_shell();
		cfg->hist_nb += 1;
		nb = ft_itoa(cfg->hist_nb);
		ft_hash_add(cfg->hist_map, nb, ft_strdup((char *)hs->data), NULL);
		ft_strdel(&nb);
	}
	else
		ft_dlstdelone(&hs);
}

void		update_history_file(t_dlist *hs, t_cfg *cfg)
{
	int		i;
	int		fd;
	char	*path;
	t_dlist	*tmp;

	if ((tmp = hs) && cfg)
	{
		path = NULL;
		ft_asprintf(&path, "%s/.%s_history", get_home(), PROJECT);
		if ((fd = open(path, O_CREAT | O_APPEND | O_WRONLY, 0666)) < 0)
		{
			ft_strdel(&path);
			return ;
		}
		ft_strdel(&path);
		while (tmp && tmp->next)
			tmp = tmp->next;
		i = cfg->hist_nb;
		while (tmp->prev && i-- > cfg->hist_start)
			tmp = tmp->prev;
		while (tmp)
		{
			if ((char *)tmp->data && ((char *)tmp->data)[0] != '\0')
			{
				ft_putstr_fd((char *)tmp->data, fd);
				if (((char *)tmp->data)[ft_strlen((char *)tmp->data) - 1] != '\n')
					ft_putstr_fd("\n", fd);
			}
			tmp = tmp->next;
		}
		close(fd);
	}
}

void		update_history(t_dlist *hs)
{
	t_cs_line	*cs;

	cs = cs_master(NULL, 0);
	if (hs && cs && cs->history)
	{
		cs->history->data = (void *)cs->old_history;
		while (hs && hs->next)
			hs = hs->next;
		if (hs && cs->input && cs->input[0] && cs->input[0] != '\n')
		{
			history_updater(cs, hs);
			ft_strdel(&cs->input);
		}
	}
}

static void init_history(t_dlist *hs, int fd, char **line)
{
    char    *nb;
    t_cfg   *cfg;
    int     i;

    if (hs && line && (cfg = cfg_shell()))
    {
        i = 0;
        while (get_next_line(fd, line) > 0 && ++i)
        {
            ft_dlstaddtail(&hs, ft_dlstnew(*line, 1));
            nb = ft_itoa(i);
            ft_hash_add(cfg->hist_map, nb, ft_strdup(*line), NULL);
            ft_strdel(&nb);
        }
		cfg->hist_nb = i;
        close(fd);
    }
}

t_dlist		*get_history(void)
{
	t_dlist	*hs;
	char	*line;
	int		fd;

	if ((hs = ft_dlstnew(NULL, 0)))
	{
		cfg_shell()->hist_nb = 0;
		cfg_shell()->hist_map = ft_hash_init(1);
		ft_asprintf(&line, "%s/.%s_history", get_home(), PROJECT);
		if ((fd = open(line, O_RDONLY)) > 0)
		{
			ft_strdel(&line);
			init_history(hs, fd, &line);
		}
		else
			ft_strdel(&line);
		cfg_shell()->hist_start = cfg_shell()->hist_nb;
	}
	return (hs);
}
