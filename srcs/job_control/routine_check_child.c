#include "sh.h"
#include "struct.h"
#include "exec.h"
#include "job_control.h"
#include <sys/types.h>
#include <sys/wait.h>

#include "ft_printf.h"


void	one_process_change(t_process *p)
{
	if (p->status == STOPPED)
	{
		if (p->ret == 20)
			ft_printf("\n\t%d Stopped(SIGTSTP)\t%s\n", p->pid, p->cmd);
		else
			ft_printf("\n\t%d Stopped(SIGSTOP)\t%s\n", p->pid, p->cmd);
	}
	else if (p->status == KILLED)
	{
		if (p->ret == 3)
			ft_printf("\n\t%d Quit(SIGQUIT)\t%s\n", p->pid, p->cmd);
		else
			ft_printf("\n\t%d Killed(SIGKILL)\t%s\n", p->pid, p->cmd);
	}
}

void	job_done(t_job *j, t_process *last_p)
{
	j->status = last_p->status;
	j->ret = last_p->ret;
	if (last_p->status == (FAILED | COMPLETED))
		ft_printf("\n[%d]\tDone(%d)\t%s\n", j->id, j->ret, j->cmd);
	else if (last_p->status == KILLED)
	{
		j->ret += 128;
		if (last_p->ret == 3)
			ft_printf("\n[%d]\tQuit(SIGQUIT)(%d)\t%s\n", j->id, j->ret, j->cmd);
		else
			ft_printf("\n[%d]\tKilled(SIGKILL)(%d)\t%s\n", j->id, j->ret, j->cmd);
	}

}

uint8_t		deep_check(t_job *j, pid_t child, int32_t wstatus)
{
	pid_t	last_child;

	last_child = child;
	update_process(j->process, child, wstatus);
	child = waitpid(-j->pgid, &wstatus, WUNTRACED | WNOHANG);
	if (child == -1)
		job_done(j, find_process_by_pid(j->process, last_child));
	else if (child > 0)
		deep_check(j, child, wstatus);
	else
		one_process_change(find_process_by_pid(j->process, last_child));
	return (TRUE);
}


int32_t			job_has_finish(void *job, void *status)
{
	t_job		*j;
	uint8_t	ending_status;

	j = job;
	ending_status = *((uint8_t *)(status));
	if (j->status & ending_status)
		return (1);
	return (0);
}

static	void	update_listjob(t_cfg *shell)
{
	uint8_t	ending_status;

	ending_status = (FAILED | COMPLETED | KILLED);
	ft_lstdelif(&shell->job, &ending_status, job_has_finish, del_struct_job);

}

void	what_s(t_cfg *shell, t_list *lstjob)
{
	pid_t	pid_child;
	t_job	*job;
	int32_t	wstatus;
	uint8_t new;

	new = 0;
	if (!shell->active_job)
		return ;
	while (lstjob)
	{
		job = lstjob->data;
		if ((pid_child = waitpid(-job->pgid, &wstatus, WUNTRACED | WNOHANG)))
			new = deep_check(job, pid_child, wstatus);
		lstjob = lstjob->next;
	}
	if (new == TRUE)
		update_listjob(shell);
	if (shell->debug)
	{
		t_list *job_debug = shell->job;
		while (job_debug)
		{
			debug_print_all(job, job->process, "check_child");
			job_debug = job_debug->next;
		}
	}
	
}
