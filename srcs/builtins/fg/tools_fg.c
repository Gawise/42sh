#include "libft.h"
#include "exec.h"
#include "struct.h"
#include "sh.h"
#include "var.h"
#include "ft_printf.h"
#include "job_control.h"
#include <sys/types.h>
#include <sys/wait.h>

void		update_prio_fg()
{
	t_list	*job;

	job = cfg_shell()->job;
	while (job)
	{
		if (((t_job *)job->data)->prio)
		{
			((t_job *)job->data)->prio -= 1;
			((t_job *)job->data)->cur = (((t_job *)job->data)->prio == 1 ? '-' : ' ');
		}
		else
		{
			((t_job *)job->data)->prio = 0;
			((t_job *)job->data)->cur = ' ';
		}
		job = job->next;
	}
}
