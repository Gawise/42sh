#include "libft.h"
#include "exec.h"
#include "struct.h"
#include "sh.h"
#include "var.h"
#include "ft_printf.h"

void	get_jstatus(t_job *job, char **stat)
{
	*stat = ft_strnew(32); // secur?
	if (job->status == 32)
		ft_strcpy(*stat, "Done");
	else if (job->status == 4)
		ft_strcpy(*stat, "Stopped(SIGSTP)");
	else
		ft_strcpy(*stat, "Running");
}

char	*j_stat(t_job *job)
{
	char	*ret;
	
	if (job->status == 32)
		ret = ft_strdup("Done");
	else if (job->status == 4)
		ret = ft_strdup("Stopped(SIGSTP)");
	else
		ret = ft_strdup("Running");
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
	char 	c;
	uint8_t	stop;

	job = cfg_shell()->job;
	stop = find_stopped_job(job);
	while (job)
	{
		j = job->data;
		c = get_curr(job, stop);
//		get_jstatus(j, &st);
		st = j_stat(j);
		if (opt == 'r')
//			ft_printf("[%d]%c %-24s %s", j->id, c, j_stat(j), j->cmd);
			ft_printf("[%d]%c %-24s %s", j->id, c, st, j->cmd);
		else
//			ft_printf("[%d]%c %d %-24s %s", j->id, c, j->pgid, j_stat(j), j->cmd);
			ft_printf("[%d]%c %d %-24s %s", j->id, c, j->pgid, st, j->cmd);
		if (j->status == RUNNING)
			ft_printf(" &");
		ft_printf("\n");
		ft_strdel(&st);
		job = job->next;
	}
}

void	print_job_opte(char opt, t_job *j, char curr)
{
	char	*st;
	
//	get_jstatus(j, &state);
	st = j_stat(j);
	if (opt == 'l')
//	{
	//	ft_printf("[%d]%c ", j->id, curr);
	//	ft_printf("%d %-24s %s", j->pgid, j_stat(j), j->cmd);
//	}
		ft_printf("[%d]%c %d %-24s %s", j->id, curr, j->pgid, st, j->cmd);
	else
//		ft_printf("[%d]%c %-24s %s", j->id, curr, j_stat(j), j->cmd);
		ft_printf("[%d]%c %-24s %s", j->id, curr, st, j->cmd);
	if (j->status == RUNNING)
		ft_printf(" &");
	ft_printf("\n");
	ft_strdel(&st);
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
				ft_printf("%d\n", j->pgid);
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
