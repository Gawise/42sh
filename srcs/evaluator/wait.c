#include "exec.h"
#include "libft.h"
#include "sh.h"
#include "ft_printf.h"
#include "job_control.h"
#include <sys/types.h>
#include <sys/wait.h>


void	call_jobcontroler(t_job *j)
{
	j->status = STOPPED;
	add_job_cfg(j);
	if (j->ret - 128 == 20)
		ft_printf("[%d]\t + Stopped(SIGTSTP)  %s\n", j->id, j->cmd);
	else
		ft_printf("[%d]\t + Stopped(SIGSTOP)  %s\n", j->id, j->cmd);
}

t_process	*find_process_by_pid(t_list *lst, pid_t child)
{
	t_process *p;

	while (lst)
	{
		p = lst->data;
		if (p->pid == child)
			return (p);
		lst = lst->next;
	}
	return (NULL);
}

t_process	*find_process_by_status(t_list *lst, uint8_t want)
{
	t_process *p;

	while (lst)
	{
		p = lst->data;
		if (p->status & want)
			return (p);
		lst = lst->next;
	}
	return (NULL);
}

void		aplylyse_wstatus(t_process *p, int wstatus)
{
	if (WIFEXITED(wstatus))
	{
		p->ret = WEXITSTATUS(wstatus);
		p->status = p->ret & (126 | 127) ? FAILED : COMPLETED;
	}
	else if (WIFSIGNALED(wstatus))
	{
		p->status = KILLED; //en attendant de config toutes les possibilites;
		p->ret = WTERMSIG(wstatus);
	}
	else if (WIFSTOPPED(wstatus))
	{
		p->status = STOPPED ; // anakyser en fonction du signal
		p->ret = WSTOPSIG(wstatus);
	}
}

void		update_process(t_list *lst, pid_t child, int wstatus)
{
	t_process *p;

	if (child == -1) //only debug ? 
	{
		return ;
	}
	p = find_process_by_pid(lst, child);
	aplylyse_wstatus(p, wstatus);
}

static void		update_job(t_job *j)
{
	t_list		*lst;
	t_process	*tmp;

	if ((tmp = find_process_by_status(j->process, STOPPED)))
	{
		j->ret = 128 + tmp->ret;
		call_jobcontroler(j);
	}
	else if ((tmp = find_process_by_status(j->process, KILLED)))
	{
		j->ret = 128 + tmp->ret;
		j->status = KILLED;
		if (tmp->ret == 2)
			ft_printf("\n");
		else if (tmp->ret == 3)
			ft_printf("\n\t%d Quit(SIGQUIT)\t%s\n", j->pgid, j->cmd);
		else
			ft_printf("\n\t%d Killed(SIGKILL)\t%s\n", j->pgid, j->cmd);
	}
	else
	{
		lst = ft_lstgettail(j->process);
		j->status = ((t_process *)(lst->data))->status;
		j->ret = ((t_process *)(lst->data))->ret;
	}
}

void		wait_process(t_job *job)
{
	pid_t		pid_child;
	int32_t		wstatus;

	while (ft_lsthave(job->process, has_running))
	{
		wstatus = 0;
		pid_child = waitpid(-job->pgid, &wstatus, WUNTRACED);
		update_process(job->process, pid_child, wstatus);
	}
	update_job(job);
	if (cfg_shell()->debug)
		debug_print_all(job, job->process, "wait ending");
	return ;
}
