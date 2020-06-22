/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fc.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ambelghi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/11 18:32:54 by ambelghi          #+#    #+#             */
/*   Updated: 2020/06/22 15:09:14 by ambelghi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "line_edition.h"
#include "libft.h"
#include <struct.h>
#include <sh.h>
#include "ft_printf.h"
#include "get_next_line.h"
#include "var.h"

uint8_t static	fc_erro(char *s)
{
	char    *usage;
	char	*usage2;

	usage = " : fc [-r][-e editor] [first[last]]\n        ";
	usage2 = "fc -l[-nr] [first[last]]\n        fc -s[old=new] [first]\n";
	ft_dprintf(2, "fc: invalid option -- %s\nusage%s%s", s, usage, usage2);
	return (-1);
}

int		range_error(void)
{
	ft_printf("fc : history specification out of range");
	return (-1);
}

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
		if ((flags & 1 && flags & 2))
			return (fc_erro((!p->av[*ac] ? p->av[*ac - 1] : p->av[*ac])));
	}
	return (flags);
}

char			*get_histfilename()
{
	char	*name;
	char	*tmp;
	char	*prefix;
	int		i;

	prefix = "/tmp";
	name = "/edit_hist";
	if (access(prefix, W_OK | R_OK) < 0)
		prefix = ".";
	i = 0;
	ft_asprintf(&name, "%s%s", prefix, name);
	prefix = name;
	while (access(prefix, F_OK) != -1 || i == 0)
	{
		i++;
		tmp = ft_itoa(i);
		ft_asprintf(&prefix, "%s_%s", name, tmp);
		ft_strdel(&tmp);
	}
	ft_strdel(&name);
	name = prefix;
	return (name);
}

char			*create_tmphist(int8_t *fl, char **av, int ac)
{
	t_point	range;
	char	*file;
	int		fd;
	int		order;
	char	*key;
	
	file = get_histfilename();
	if (!file || (fd = open(file, O_CREAT | O_RDONLY | O_WRONLY, 0666)) < 0)
		return (NULL);
	range = get_range(fl, av, ac, cfg_shell());
	if (*fl & 8)
		range = (t_point){range.y, range.x};
	order = (range.x > range.y ? -1 : 1);
	while (range.x - order != range.y && range.x > 0)
	{
		key = ft_itoa(range.x);
		ft_putstr_fd((char *)ft_hash_lookup(cfg_shell()->hist_map, key), fd);
		ft_putstr_fd("\n", fd);
		ft_strdel(&key);
		range.x += order;
	}
	close(fd);
	return (file);
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
	while (n && ret++ >= 0)
		n /= 10;
	return (ret);
}

int			print_hist(int8_t *fl, char **av, int ac)
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
		return (1);
	}
	return (-1);
}

void		exec_hist(int8_t fl, char *file)
{
	t_lexer     lexer;
	t_parser    parser;
	char		*cmd;
	char		*hist_cmd;
	int			fd;
	int			ret;
	char		*nb;

	if (file && (fd = open(file, O_RDONLY)) >= 0)
	{
		while (get_next_line(fd, &cmd) > 0)
		{
			printf("cmd : %s\n", cmd);
			hist_cmd = ft_strdup(cmd);
			if ((ret = lexer_routine(&cmd, &lexer)) <= 0
					|| (ret = parser_routine(&lexer, &parser)) <= 0
					|| (ret = eval_routine(&parser)) <= 0)
					break ;
			if (!(fl & 1) && ft_atoi(find_var_value(cfg_shell()->sp, "?")) == 0)
			{
				cfg_shell()->hist_nb += 1;
				nb = ft_itoa(cfg_shell()->hist_nb);
				ft_dlstaddtail(&cfg_shell()->history, ft_dlstnew(hist_cmd, 0));
				ft_hash_add(cfg_shell()->hist_map, nb, ft_strdup(hist_cmd), NULL);
				ft_strdel(&nb);
			}
		}
	}
}

int			edit_hist(int8_t *fl, char **av, int ac)
{
	char	*hist;
	char	*cmd;
	t_lexer     lexer;
	t_parser    parser;
	int			ret;

	if ((hist = create_tmphist(fl, av, (av[ac] ? ac + 1 : ac))))
	{
		if ((!av[ac] || !ft_strisalpha(av[ac])) && !find_var_value(
			cfg_shell()->sp, "FCEDIT"))
			ft_asprintf(&cmd, "%s %s", "ed", hist);
		else if (!av[ac] || !ft_strisalpha(av[ac]))
			ft_asprintf(&cmd, "%s %s", find_var_value(cfg_shell()->sp,
						"FCEDIT"), hist);
		else
			ft_asprintf(&cmd, "%s %s", av[ac], hist);
		if ((ret = lexer_routine(&cmd, &lexer)) <= 0
				|| (ret = parser_routine(&lexer, &parser)) <= 0
				|| (ret = eval_routine(&parser)) <= 0)
			return (0);
		exec_hist(*fl, hist);
		ft_strdel(&hist);
		return (1);
	}
	return (0);
}

uint8_t			ft_fc(t_job *j, t_process *p)
{
	int8_t	fl;
	int32_t	ac;
	t_dlist	*hs;

	ac = 1;
	if ((fl = check_opt(p, &ac)) < 0)
		return (FAILURE);
	if (j && p)
	{
		if (fl & 1)
		{
			hs = cfg_shell()->history;
			while (hs->next)
				hs = hs->next;
			ft_dlstdelone(&hs);
		}
		if (fl & 1 && !edit_hist(&fl, p->av, ac))
			return (FAILURE);
		if (fl & 2 && !print_hist(&fl, p->av, ac))
			return (FAILURE);
		return (SUCCESS);
	}
	return (SUCCESS);
}
