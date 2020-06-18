#include "libft.h"
#include "ft_printf.h"
#include "exec.h"
#include "sh.h"


# define curr_job1 %%
# define curr_job2 %+
# define prev_job %-
# define nb_job %n
# define begin_job %string
# define contain_job %?string

static void			print_this_job(t_job *j, uint8_t curr, int16_t opt)
{
	if (curr > 45)
		curr = 32;
	ft_printf("%s %d %d %d\n", j->cmd, j->id, curr, opt);
}

static void			print_all_jobs(t_cfg *shell, t_list *jobs, int16_t opt)
{
	uint8_t		i;
	uint8_t		c;
	t_job		*j;
	t_list		*tmp;

	i = 0;
	while (i++ < shell->active_job)
	{
		c = 43;
		tmp = jobs;
		while (tmp && (j = tmp->data) && j->id != i && (c += 2))
			tmp = tmp->next;
		if (tmp)
			print_this_job(j, c, opt);
	}
	ft_printf(" %d < %d\n", i, shell->active_job);
}


static int16_t		jobs_opt(char **av, int *ac)
{
	int32_t		i;
	int32_t		ret;
	uint8_t		opt;

	i = 0;
	opt = 0;
	while ((ret = ft_getopt(ac, &i, av, "pl") != -1))
	{
		//if (ret == '?')
		//	error();
		opt = ret;
	}
	return (opt);
}

uint8_t		ft_jobs2(t_job *j, t_process *p)
{
	int32_t		ac;
	int16_t		opt;
	t_cfg		*shell;

	shell = cfg_shell();
	(void)j;
	ac = 1;
	opt = jobs_opt(p->av, &ac);
//	if (p->av[ac])
//		print_job();
//	else
		print_all_jobs(shell, shell->job, opt);

	return (0);
}
