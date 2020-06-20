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


static int8_t			error(void)
{
	ft_printf("error\n");
	return (FAILURE);
}

static char			*state(t_job *j, uint8_t opt)
{
	char	*tab[4];
	int16_t	s_ret;

	tab[0] = S_SIGSTOP;
	tab[1] = S_SIGTSTP;
	tab[2] = S_SIGTTIN;
	tab[3] = S_SIGTTOU;
	if (j->status & RUNNING)
		return ("Running");
	s_ret = j->ret - 19 - 128;
	if (s_ret > -1 && s_ret < 4 && !opt)
		return (tab[s_ret]);
	if (j->status & STOPPED)
		return ("Stopped");
	return ("unknown");
}

static void			print_this_job(t_job *j, uint8_t curr, char opt)
{
	curr = (curr > 45) ? 32 : curr;
	if (opt == 'p')
		ft_printf("%d\n", j->pgid);
	else if (opt == 'l')
		ft_printf("[%d]%c %d %s \t %s\n", j->id, curr, j->pgid, state(j, 1), j->cmd);
	else
		ft_printf("[%d]%c %s \t %s\n", j->id, curr, state(j, 0), j->cmd);
}

static void			print_all_jobs(t_cfg *shell, t_list *jobs, char opt)
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
}

static char		jobs_opt(char **av, int *ac)
{
	int32_t		i;
	int8_t		ret;
	char		opt;

	i = 0;
	opt = 0;
	while ((ret = ft_getopt(ac, &i, av, "pl")) != -1)
	{
		if (ret == '?')
			return (error());
		opt = ret != -1 ? ret : opt;
	}
	return (opt);
}

uint8_t		ft_jobs2(t_job *j, t_process *p)
{
	int32_t		ac;
	char		opt;
	t_cfg		*shell;

	shell = cfg_shell();
	(void)j;
	ac = 1;
	if ((opt = jobs_opt(p->av, &ac)) == FAILURE)
		return (FAILURE);
	
//	if (p->av[ac])
//		print_job();
//	else
		print_all_jobs(shell, shell->job, opt);

	return (0);
}
