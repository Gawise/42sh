#include "libft.h"
#include "exec.h"
#include "struct.h"
#include "sh.h"
#include "var.h"
#include "ft_printf.h"


void	get_jstatus(t_job *job, char **stat)
{
	*stat = ft_strnew(32);

	if (job->status == 32)
		ft_strcpy(*stat, "Done");
	else if (job->status == 4)
		ft_strcpy(*stat, "Stopped(SIGSTP)");
	else
		ft_strcpy(*stat, "Running");
}

 // The character '+' identifies the job that would be used as a default for the fg or bg utilities; this job can also be specified using the job_id %+ or "%%". The character '-' identifies the job that would become the default if the current default job were to exit; this job can also be specified using the job_id %-. For other jobs, this field is a <space>. At most one job can be identified with '+' and at most one job can be identified with '-'. If there is any suspended job, then the current job shall be a suspended job. If there are at least two suspended jobs, then the previous job also shall be a suspended job.
int8_t		find_stopped_job(t_list *job)
{
	int	i;

	i = 0;
	while (job)
	{
		if (((t_job *)job->data)->status == STOPPED)
			i++;
		job = job->next;
	}
	return (i);
}

char		get_curr_multi(t_list *job, uint8_t nb)
{
	if (((t_job *)job->data)->status != STOPPED)
		return (' ');
	else
	{
		if (nb == 1)
			return ('+');
		else if (nb == 2)
			return ('-');
		else
			return (' ');
	}
}

char		get_curr(t_list *job, uint8_t stop)
{
	uint8_t	nb;

	nb = find_stopped_job(job);
	if (stop == 0)
	{
		if (!job->next)
			return ('+');
		else if (!(job->next)->next)
			return ('-');
		else
			return (' ');
	}
	else if (stop == 1)
	{
		if (((t_job *)job->data)->status == STOPPED)
			return ('+');
		else if ((!job->next)  || (!(job->next)->next &&
		(((t_job *)(job->next)->data)->status == STOPPED)))
			return ('-');
		else
			return (' ');
	}
	else // plusieurs job suspendus
		return (get_curr_multi(job, nb));
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

void		print_jobs_opt(t_job *j)
{
	t_list	*job;

	job = cfg_shell()->job;
	while (job)
	{
		j = job->data;
		ft_printf("%d\n", j->pgid);
		job = job->next;
	}

}

void		print_jobs(char opt, t_job *j)
{
	t_list	*job;
	char	*state;
	char 	curr;
	uint8_t	stop;

	job = cfg_shell()->job;
	stop = find_stopped_job(job);
	while (job)
	{
		j = job->data;
		curr = get_curr(job, stop);
		get_jstatus(j, &state);
		if (opt == 'r')
			ft_printf("[%d]%c %-24s %s\n", j->id, curr, state, j->cmd);
		else
			ft_printf("[%d]%c %d %-24s %s\n", j->id, curr, j->pgid, state, j->cmd);
		ft_strdel(&state);
		job = job->next;
	}

}

int		str_is_digit(char *ope)
{
	while (*ope)
		if (!ft_isdigit(*ope++))
			return (0);
	return (1);
}

int8_t		get_curr_id(void)
{
	int8_t	s;
	t_list	*j;

	j = cfg_shell()->job;
	s = find_stopped_job(j);
	while (j)
	{
		if ((!s && !j->next) ||
		(s == 1 && (((t_job *)j->data)->status == STOPPED)) ||
		(s > 1 && find_stopped_job(j) == 1 &&
		(((t_job *)j->data)->status == STOPPED)))
			return (((t_job *)j->data)->id);
		j = j->next;
	}
	return (0);
}

int8_t		get_prev_id(void)
{
	int8_t	s;
	t_list	*j;

	j = cfg_shell()->job;
	s = find_stopped_job(j);
	if (!j->next)
		return (((t_job *)j->data)->id);
	while (j)
	{
		if ((!s && !(j->next)->next) ||
		(s == 1 && ((!j->next && (((t_job *)j->data)->status != STOPPED))
		|| (!(j->next)->next && (((t_job *)(j->next)->data)->status == STOPPED)))) ||
		(s > 1 && ((t_job *)j->data)->status == STOPPED && 
		find_stopped_job(j) == 2))
			return (((t_job *)j->data)->id);
		j = j->next;
	}
	return (0);
}

int8_t		get_sstr_id(char *ope)
{
	t_list	*job;
	int8_t	mm;
	int8_t	id;

	mm = 0;
	id = 0;
	job = cfg_shell()->job;
	while (job)
	{
		if (ft_strstr(((t_job *)job->data)->cmd, ope))
		{
			id = (((t_job *)job->data)->id);
			if (++mm > 1)
				break;
		}
		job = job->next;
	}
	if (mm == 1)
		return (id);
	else
		return (0); // gerer les msg erreurs differents!
	return 0;
}

int8_t		get_str_id(char *ope)
{
	t_list	*job;
	int8_t	mm;
	int8_t	id;
	int8_t	i;

	mm = 0;
	id = 0;
	job = cfg_shell()->job;
	while (job)
	{
		i = 0;
		while (ope[i] && ope[i] == ((t_job *)job->data)->cmd[i])
			i++;
		if (ope[i] == '\0')
		{
			mm++;
			id = (((t_job *)job->data)->id);
		}
		job = job->next;
	}
	if (mm == 1)
		return (id);
	else
		return (0); // gerer les msg erreurs differents!
}

uint8_t		get_job_id(char *ope)
{
	int8_t	ret;

	ret = 0;
	ft_printf("ope = %s\n", ope);
	if (!ft_strcmp(ope, "%"))
		ret = get_curr_id();
	else if (*ope == '%')
		ope++;
	if (str_is_digit(ope))
		ret = ft_atoi(ope);
	else if (*ope == '+' || *ope == '%')
		ret = get_curr_id();
	else if (*ope == '-')
		ret = get_prev_id();
	else if (*ope == '?')
		ret = get_sstr_id(++ope);
	else
		ret = get_str_id(ope);
	return (ret);
}

void	print_job_opte(char opt, t_job *j, char curr)
{
	char	*state;

	get_jstatus(j, &state);
	if (opt == 'l')
		ft_printf("[%d]%c %d %-24s %s\n", j->id, curr, j->pgid, state, j->cmd);
	else
		ft_printf("[%d]%c %-24s %s\n", j->id, curr, state, j->cmd);
}

int	print_job_ope(char opt, t_job *j, int8_t ope)
{
	t_list	*job;
	int8_t	stop;
	char	curr;

	job = cfg_shell()->job;
	stop = find_stopped_job(job);
	while (job)
	{
		j = job->data;
		if (j->id == ope)
		{
			if (opt == 'p')
				ft_printf("%d %s\n", j->pgid);
			else
			{
				curr = get_curr(job, stop);
				print_job_opte(opt, j, curr);
			}
			return (1);
		}
		job = job->next;
	}
	return (0);
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
