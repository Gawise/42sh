#include "libft.h"
#include "exec.h"
#include "struct.h"
#include "sh.h"
#include "var.h"
#include "ft_printf.h"


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

char		opt_jobs(char **av, char *flags, int *ac)
{
	char	opt;
	int	i;

	opt = 'r';
	i = 0;
	while (av[*ac] && av[*ac][i] == '-')
	{
		i++;
		if (av[*ac][i] == '-')
			return (opt);
		while (av[*ac][i])
		{
			if ((opt = ft_check_opt(av[*ac][i], flags)) == '?')
				return (opt);
			i++;
		}
		*ac += 1;
		i = 0;
	}
	return (opt);
}

uint8_t 	ft_jobs(t_job *j, t_process *p)
{
	char	opt;
	int	ac;
	int8_t	ope;

	ac = 1;
	if ((opt = opt_jobs(p->av, "lp", &ac)) == '?')
	{
		ft_dprintf(STDERR_FILENO, "21sh: jobs: invalid option\njobs");
		ft_dprintf(STDERR_FILENO, " : usage: jobs [-lp] [job_id]\n");
		return (0);
	}
	if (p->av[ac])
	{
		ope = get_job_id(p->av[ac]);
		if (!print_job_ope(opt, j, ope))
			ft_dprintf(STDERR_FILENO, "21sh: jobs: %s:no such job\n", p->av[ac]);
		return 0;
	}
	if (opt == 'r' || opt == 'l')	
		print_jobs(opt, j);
	else
		print_jobs_opt(j);
	return (0);
}
