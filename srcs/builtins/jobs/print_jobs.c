#include "libft.h"
#include "exec.h"
#include "struct.h"
#include "sh.h"
#include "ft_printf.h"
#include <signal.h>

char	*j_stat(t_job *j)
{
	char	*ret;

	if (j->status == 32)
		ret = ft_strdup("Done");
	else if (j->status == 4)
		ret = ft_strdup("Stopped(SIGTSTP)");
	else
		ret = ft_strdup("Running");
	protect_malloc(ret);
	return (ret);
}

void	print_jobs_opt(t_job *j)
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

void	print_jobs(char opt, t_job *j)
{
	t_list	*job;
	char	*st;

	job = cfg_shell()->job;
	while (job)
	{
		j = job->data;
		st = j_stat(j);
		protect_malloc(st);
		if (opt == 'r')
			ft_printf("[%d]%c %-24s %s", j->id, j->cur, st, j->cmd);
		else
		{
			ft_printf("[%d]%c %d ", j->id, j->cur, j->pgid);
			ft_printf("%-24s %s", st, j->cmd);
		}
		if (j->status == RUNNING)
			ft_printf(" &");
		ft_printf("\n");
		ft_strdel(&st);
		job = job->next;
	}
}

void	print_job_opte(char opt, t_job *j)
{
	char	*st;

	st = j_stat(j);
	protect_malloc(st);
	if (opt == 'l')
		ft_printf("[%d]%c %d %-24s %s", j->id, j->cur, j->pgid, st, j->cmd);
	else
		ft_printf("[%d]%c %-24s %s", j->id, j->cur, st, j->cmd);
	if (j->status == RUNNING)
		ft_printf(" &");
	ft_printf("\n");
	ft_strdel(&st);
}

int		print_job_ope(char opt, t_job *j, int8_t ope)
{
	t_list	*job;

	job = cfg_shell()->job;
	while (job)
	{
		j = job->data;
		if (j->id == ope)
		{
			if (opt == 'p')
				ft_printf("%d\n", j->pgid);
			else
				print_job_opte(opt, j);
			return (1);
		}
		job = job->next;
	}
	return (0);
}
