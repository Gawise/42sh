/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fc.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ambelghi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/11 18:32:54 by ambelghi          #+#    #+#             */
/*   Updated: 2020/07/06 18:57:55 by ambelghi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "line_edition.h"
#include "libft.h"
#include <struct.h>
#include <sh.h>
#include "ft_printf.h"
#include "get_next_line.h"
#include "var.h"

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

uint8_t		protect_fd(int8_t value)
{
	static uint8_t	protect = 0;

	if (value > -1)
		protect = value;
	return (protect);

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
		if (!(*fl & 1) && !find_var_value(cfg_shell()->env, "FCEDIT"))
			ft_asprintf(&cmd, "%s %s", "ed", hist);
		else if (!(*fl & 1))
			ft_asprintf(&cmd, "%s %s", find_var_value(cfg_shell()->env,
						"FCEDIT"), hist);
		else if (av[ac])
			ft_asprintf(&cmd, "%s %s", av[ac], hist);
		else
			return (fc_missing_arg());
		if ((ret = lexer_routine(&cmd, &lexer)) <= 0
				|| (ret = parser_routine(&lexer, &parser)) <= 0
				|| (ret = eval_routine(&parser)) <= 0)
			return (0);
		if (ft_atoi(find_var_value(cfg_shell()->sp, "?")) == 0)
			exec_hist(hist);
		ft_strdel(&hist);
		return (1);
	}
	return (0);
}

char			*get_hist_cmd(char *av, t_cfg *cfg)
{
	char	*cmd;
	char	*nb;
	int		n;
	t_dlist	*hs;
	int		i;

	cmd = NULL;
	if (av)
	{
		if (ft_strisdigit(av))
		{
			n = ft_atoi(av);
			n = (n < 0 ? cfg->hist_nb + n : n);
			nb = ft_itoa(n + 1);
			cmd = ft_strdup((char *)ft_hash_lookup(cfg->hist_map, nb));
			ft_strdel(&nb);
		}
		else if ((hs = cfg->history))
		{
			while (hs->next)
				hs = hs->next;
			n = ft_strlen(av);
			while (hs->prev)
			{
				i = 0;
				while (hs->data && ((char *)hs->data)[i] && ((char *)hs->data)[i] == ' ')
					i++;
				if (hs->data && ft_strnstr(&((char *)hs->data)[i], av, n))
					return (ft_strdup((char *)hs->data));
				hs = hs->prev;
			}
		}
	}
	else
	{
		nb = ft_itoa(cfg->hist_nb);
		cmd = ft_strdup((char *)ft_hash_lookup(cfg->hist_map, nb));
		ft_strdel(&nb);
	}
	return (cmd);
}

void			reexecute_cmd(char **av, int ac)
{
	char	*old;
	char	*new;
	char	*oc;
	char	*cmd;

	old = NULL;
	new = NULL;
	if (av[ac] && (oc = ft_strchr(av[ac], '=')))
	{
		*oc = '\0';
		old = av[ac];
		new = oc + 1;
		ac++;
	}
	if ((cmd = get_hist_cmd(av[ac], cfg_shell())))
	{
		if (old)
		{
			oc = cmd;
			cmd = ft_strrep(cmd, old, new);
			ft_strdel(&oc);
		}
		oc = get_histfilename();
		if (!oc || (ac = open(oc, O_CREAT | O_WRONLY, 0666)) < 0)
			return ;
		ft_dprintf(ac, "%s", cmd);
		close(ac);
		exec_hist(oc);
	}
}

uint8_t			ft_fc(t_job *j, t_process *p)
{
	int8_t	fl;
	int32_t	ac;
	t_dlist	*hs;
	char	*nb;

	ac = 1;
	if ((fl = fc_check_opt(p, &ac)) < 0)
		return (FAILURE);
	if (j && p)
	{
		if (fl & 1 || fl & 16)
		{
			hs = cfg_shell()->history;
			while (hs->next)
				hs = hs->next;
			nb = ft_itoa(cfg_shell()->hist_nb);
			ft_dlstdelone(&hs);
			ft_hash_delone(cfg_shell()->hist_map, nb, free);
			cfg_shell()->hist_nb -= 1;
			ft_strdel(&nb);
		}
		if (((!(fl & 2) && !(fl & 16)) || fl & 1) && !edit_hist(&fl, p->av, ac))
			return (FAILURE);
		if (fl & 2 && !print_hist(&fl, p->av, ac))
			return (FAILURE);
		if (fl & 16)
			reexecute_cmd(p->av, ac);
		return (SUCCESS);
	}
	return (SUCCESS);
}
