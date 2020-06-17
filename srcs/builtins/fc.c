/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fc.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ambelghi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/11 18:32:54 by ambelghi          #+#    #+#             */
/*   Updated: 2020/06/17 18:44:05 by ambelghi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "line_edition.h"
#include "libft.h"
#include <struct.h>
#include <sh.h>
#include "ft_printf.h"

uint8_t static	fc_erro(char *s)
{
	char    *usage;
	char	*usage2;

    usage = " : fc [-r][-e editor] [first[last]]\n        ";
	usage2 = "fc -l[-nr] [first[last]]\n        fc -s[old=new] [first]\n";
    ft_dprintf(2, "fc: illegal option -- %s\nusage%s%s", s, usage, usage2);
    return (-1);
}

int		range_error(void)
{
	ft_printf("fc : history specification out of range");
	return (-1);
}

static uint8_t  check_opt(t_process *p, int32_t *ac)
{
    int32_t     i;
	int			z;
    int8_t      ret;
    int8_t      flags;

    flags = 0;
    i = 0;
	char *fl = "elnrs";
    while ((ret = ft_getopt(ac, &i, p->av, fl)) != -1)
    {
        if (ret == '?')
		{
			z = i;
			while (p->av[*ac][z])
				if (!ft_isdigit(p->av[*ac][z++]))
					return (fc_erro(&p->av[*ac][i]));
			break ;
		}
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
/*
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
*/
static t_point         get_range(int8_t *fl, char **av, int ac, t_cfg *cfg)
{
    t_point t;

    if (av && av[ac])
    {
        if (!av[ac + 1] || (t.y = ft_atoi(av[ac + 1])) == 0 || t.y > cfg->hist_nb)
            t.y = cfg->hist_nb - 1;
        if (t.y < 0)
            t.y = cfg->hist_nb + t.y;
		if ((t.x = ft_atoi(av[ac])) == 0 || t.x > cfg->hist_nb)
            t.x = cfg->hist_nb - 1;
        if (t.x < 0)
            t.x = cfg->hist_nb + t.x;
        if (t.x > t.y)
        {
            t = (t_point){t.y, t.x};
            *fl = (*fl & 8 ? *fl ^ 8 : *fl | 8);
        }
    }
    else
    {
		(void)fl;
        t.y = cfg->hist_nb - 1;
        t.x = (cfg->hist_nb <= 15 ? 1 : cfg->hist_nb - 15);
    }
    return (t);
}

static int		check_range(char **av, int ac)
{
	int	i;
	int	z;
	if (av && av[ac])
	{
		i = ac;
		while (av[i])
		{
			z = 0;
			if (av[i][z] == '-')
				z = 1;
			while (av[i][z])
				if (!ft_isdigit(av[i][z++]))
					return (range_error());
			i++;
		}
	}
	return (1);
}

static int		ft_intlen(int nb)
{
	unsigned int	n;
	int				ret;

	if (nb < 0)
		n = nb * (-1);
	else
		n = (unsigned int)nb;
	if (n == 0)
		return (1);
	ret = 0;
	while (n && ret++)
		n /= 10;
	return (ret);
}

void			print_hist(int8_t *fl, char **av, int ac)
{
	t_point	range;
	int		order;
	t_cfg	*cfg;
	char	*key;

	if (av && (cfg = cfg_shell()) && check_range(av, ac) == 1)
	{
		range = get_range(fl, av, ac, cfg);
		if (*fl & 8)
			range = (t_point){range.y, range.x};
		order = (range.x > range.y ? -1 : 1);
		while (range.x - order != range.y && range.x > 0)
		{
			if (!(*fl & 4))
                ft_putnbr(range.x);
            putchar_n(' ', (!(*fl & 4) ? 12 - ft_intlen(range.x) : 12));
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
			print_hist(&fl, p->av, ac);
		return (SUCCESS);
	}
	return (SUCCESS);
}
