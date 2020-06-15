/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fc.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ambelghi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/11 18:32:54 by ambelghi          #+#    #+#             */
/*   Updated: 2020/06/15 22:43:35 by ambelghi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <struct.h>
#include <sh.h>
#include "ft_printf.h"

uint8_t static	fc_erro(char c)
{
	char    *usage;
	char	*usage2;
	//char	*usage3;

    usage = " : fc [-r][-e editor] [first[last]]\n        ";
	usage2 = "fc -l[-nr] [first[last]]\n        fc -s[old=new] [first]\n";
    ft_dprintf(2, "fc: illegal option -- %c\nusage%s%s", c, usage, usage2);
    return (-1);
}

static uint8_t  check_opt(t_process *p, int32_t *ac)
{
    int32_t     i;
    int8_t      ret;
    int8_t      flags;

    flags = 0;
    i = 0;
	char *fl = "elnrs";
    while ((ret = ft_getopt(ac, &i, p->av, fl)) != -1)
    {
        if (ret == '?')
            return (fc_erro(p->av[*ac][i]));
		flags |= (char)ret == 'e' ? 1 : 0;
		flags |= (char)ret == 'l' ? 2 : 0;
		flags |= (char)ret == 'n' ? 4 : 0;
		flags |= (char)ret == 'r' ? 8 : 0;
		flags |= (char)ret == 's' ? 16 : 0;
    }
    return (flags);
}

char			*create_histfile()
{
	if (access("/tmp", W_OK | R_OK) < 0)
		return (ft_strdup("./edit_hist"));
	else
		return (ft_strdup("/tmp/edit_hist"));
}

char			*init_file(t_cfg *cfg, t_point cmd_nb)
{
	t_dlist	*hist;
	char	*filename;
	int		i;
	int		fd;

	filename = create_histfile();
	hist = cfg->history;
	i = 0;
	if ((fd = open(filename, O_CREAT | O_RDONLY | O_WRONLY, 0666)) < 0)
		return (NULL);
	while (cfg->hist_nb - i++ != cmd_nb.x && hist->prev)
		hist = hist->prev;
	i = 0;
	while (i++ < cmd_nb.y && hist)
	{
		ft_putstr_fd((char *)hist->data, fd);
		ft_putstr_fd("\n", fd);
		hist = hist->next;
	}
	close(fd);
	return (filename);
}

void			print_hist(int8_t fl, char **av, int ac)
{
	t_point	range;
	int		order;
	t_cfg	*cfg;
	char	*key;

	if (av && (cfg = cfg_shell()))
	{
		range = (t_point){0, 0};
		range.x = (av[ac] ? ft_atoi(av[ac]) : 0);
		range.y = (av[ac] && av[ac + 1] ? ft_atoi(av[ac + 1]) : cfg->hist_nb - 20);
		range.y = (range.y < 0 ? 1 : range.y);
		order = (range.x > range.y ? -1 : 1);
		range.x = (range.x == 0 ? cfg->hist_nb : range.x);
		range.y = (range.y == 0 && range.x != cfg->hist_nb ? cfg->hist_nb : range.y);
		if (fl & 8)
			range = (t_point){range.y, range.x};
		while (range.x - order != range.y && range.x < cfg->hist_nb && range.x > 0)
		{
			if (!(fl & 4))
                ft_putnbr(range.x);
            ft_putstr((fl & 4 ? "       " : "     "));
            key = ft_itoa(range.x);
			ft_putendl((char *)ft_hash_lookup(cfg->hist_map, key));
			ft_strdel(&key);
			range.x += order;
		}
	}
}

uint8_t			ft_fc(t_job *j, t_process *p)
{
	int8_t	fl;
	int32_t	ac;

	ac = 1;
	if ((fl = check_opt(p, &ac)) < 0)
        return (FAILURE);
	if (j && p)
	{
		if (fl & 2)
			print_hist(fl, p->av, ac);
		return (SUCCESS);
	}
	return (SUCCESS);
}
