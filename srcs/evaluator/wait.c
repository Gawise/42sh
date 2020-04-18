#include "exec.h"
#include "libft.h"
#include "sh.h"
#include <sys/types.h>
#include <sys/wait.h>

void	call_jobcontroler(t_job *j)
{
	(void)j;
	printf("[var intern] Stopped(SIGTSTP)  ->commande line used for message\n");
}


int32_t	has_running(t_list *lst)
{
	t_process *p;

	p = lst->data;
	if (p->status == RUNNING)
		return (1);
	return (0);
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

static uint8_t		aplylyse_wstatus(t_process *p, int wstatus)
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
		return (1);
	}
	return (0);
}

static uint8_t	update_process(t_list *lst, pid_t child, int wstatus)
{
	t_process *p;

	p = find_process_by_pid(lst, child);
	return (aplylyse_wstatus(p, wstatus));
}

void		update_job(t_job *j, uint8_t stopped)
{
	t_list		*lst;

	if (stopped)
	{
		j->ret = 128 + find_process_by_status(j->process, STOPPED)->ret;
		call_jobcontroler(j);
		return ;
	}
	lst = ft_lstgettail(j->process);
	j->status = ((t_process *)(lst->data))->status;
	if (j->status & (COMPLETED | FAILED))
		j->ret = ((t_process *)(lst->data))->ret;
	else if (j->status & KILLED)
		j->ret = 128 + ((t_process*)(lst->data))->ret;
}

void		wait_process(t_job *job)
{
	pid_t		pid_child;
	int32_t		wstatus;
	uint8_t		stopped;

	stopped = 0;
	while (ft_lsthave(job->process, has_running))
	{
		wstatus = 0;
		pid_child = waitpid(-job->pgid, &wstatus, WUNTRACED);
		if (pid_child == -1)
			perror("[WAIT_PROCESS] error waitpid"); //perror
		if (update_process(job->process, pid_child, wstatus))
			stopped = TRUE;
	}
	update_job(job, stopped);
	if (cfg_shell()->debug)
		debug_print_all(job, job->process, "wait ending");
	return ;
}

