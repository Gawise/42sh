#include "libft.h"
#include "exec.h"
#include "struct.h"
#include "sh.h"
#include "var.h"
#include "ft_printf.h"

int		print_jobs_error(char c)
{
	ft_dprintf(STDERR_FILENO, "21sh: jobs: -%c: invalid option\n", c);
	ft_dprintf(STDERR_FILENO, "jobs : usage: jobs [-lp] [job_id...]\n");
	return (2);
}

char	ft_check_opt(char av, char *flags)
{
	while (*flags)
	{
		if (*flags == av)
			return (av);
		flags++;
	}
	return ('?');
}

char	opt_jobs(char **av, char *flags, int *ac, int *i)
{
	char	opt;

	opt = 'r';
	while (av[*ac] && av[*ac][*i] == '-')
	{
		if (!av[*ac][1])
			return (opt);
		*i += 1;
		if ((av[*ac][*i] == '-' && av[*ac][*i + 1]))
			return ('?');
		if ((av[*ac][*i] == '-' && (*ac += 1)))
			return (opt);
		while (av[*ac][*i])
		{
			if ((opt = ft_check_opt(av[*ac][*i], flags)) == '?')
				return (opt);
			*i += 1;
		}
		*ac += 1;
		*i = 0;
	}
	return (opt);
}

uint8_t	ft_multi_jobs(t_job *j, char **tab, int ac, char opt)
{
	uint8_t	ret;
	int16_t	ope;

	ret = 0;
	while (tab[ac])
	{
		ope = get_job_id(tab[ac]);
		if (!print_job_ope(opt, j, ope))
		{
			if (ope == -1)
			{
				ft_dprintf(STDERR_FILENO, "21sh: jobs: %s", tab[ac]);
				ft_dprintf(STDERR_FILENO, ": ambiguous job spec\n");
			}
			ft_dprintf(STDERR_FILENO, "21sh: jobs: %s", tab[ac]);
			ft_dprintf(STDERR_FILENO, ": no such job\n");
			ret++;
		}
		ac++;
	}
	return (ret ? 1 : 0);
}

uint8_t	ft_jobs(t_job *j, t_process *p)
{
	char	opt;
	int		ac;
	int		i;

	ac = 1;
	i = 0;
	if ((opt = opt_jobs(p->av, "lp", &ac, &i)) == '?')
		return (print_jobs_error(p->av[ac][i]));
	if (p->av[ac])
		return (ft_multi_jobs(j, p->av, ac, opt));
	if (opt == 'r' || opt == 'l')
		print_jobs(opt, j);
	else
		print_jobs_opt(j);
	return (0);
}
