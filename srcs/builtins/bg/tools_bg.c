#include "libft.h"
#include "exec.h"
#include "struct.h"
#include "sh.h"
#include "var.h"
#include "ft_printf.h"
#include "job_control.h"
#include <sys/types.h>
#include <sys/wait.h>

void		last_prio(int pos)
{
	t_list *job;
	int		stop;

	job = cfg_shell()->job;
	while (job)
	{
		stop = find_stopped_job(job);
		if (((t_job *)job->data)->prio == 0 &&
		(((t_job *)job->data)->status == STOPPED) && stop == pos)
		{
			((t_job *)job->data)->prio = 1; //?,
			return ;
		}
		job = job->next;
	}
}

void		update_last_prio(t_list *job)
{
	int	n;

	while (job)
	{
		n = find_stopped_job(job);
		if (((t_job *)job->data)->prio)
		{
			((t_job *)job->data)->prio = 2;
			if (n == 1)
				return (last_prio(2));
			else
				return (last_prio(1));
		}
		job = job->next;
	}
}

void		update_prio_bg()
{
	t_list	*job;
	int		stop;

	job = cfg_shell()->job;	
	if ((stop = find_stopped_job(job)) <= 1)
		return ;
	else if (stop == 2)
	{
		while (job)
		{
			if (((t_job *)job->data)->status == RUNNING)
				((t_job *)job->data)->prio = 0;
			else if (((t_job *)job->data)->status == STOPPED &&
				((t_job *)job->data)->prio != 2)
				((t_job *)job->data)->prio += 1;
			job = job->next;
		}
	}
	else
		update_last_prio(job);
}
