#include "exec.h"
#include "libft.h"
#include <sys/types.h>
#include <sys/wait.h>

void	call_jobcontroler(t_job *j)
{
	(void)j;
	printf("[nb job] Stopped(SIGTSTP)  ->commande line used for message\n");
}


int			has_running(t_list *lst)
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

int		aplylyse_wstatus(t_process *p, int wstatus)
{
	if (WIFEXITED(wstatus))
	{
		p->ret = WEXITSTATUS(wstatus);
		p->status = (p->ret == (126 || 127)) ? FAILED : COMPLETED;
//		p->status = COMPLETED;
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
	return (p->status);
}

int		update_process(t_list *lst, pid_t child, int wstatus)
{
	t_process *p;

	p = find_process_by_pid(lst, child);
	return (aplylyse_wstatus(p, wstatus));
}

void		update_job(t_job *j)
{
	t_list		*lst;

	if (j->status == COMPLETED)
	{
		lst = ft_lstgettail(j->process);
		j->ret = ((t_process *)(lst->data))->ret;
	}
	if (j->status == KILLED)
		j->ret = 130;
	if (j->status == STOPPED)
	{
		j->ret = 146;
		call_jobcontroler(j);
	}
}

int		wait_process(t_job *job)
{
	pid_t		pid_child;
	int			wstatus;

	while (ft_lsthave(job->process, has_running))
	{
		wstatus = 0;
		pid_child = waitpid(-job->pgid, &wstatus, WUNTRACED);
		if (pid_child == -1)
			ex("[WAIT_PROCESS] error waitpid");
		job->status = update_process(job->process, pid_child, wstatus);
	}
	update_job(job);




	/* DEBUG  */
		t_process *process;
		t_list *j = job->process;
		printf("\n\n ----------------------\n--> [INFO PROCESS] \n");
		while (j)
		{
			process = j->data;
			printf("cmd = [%s]\t retour = [%d]\t status = [%d]\n", process->path, process->ret, process->status);
			j = j->next;
		}
		printf("--> [INFO JOB] \n");
		printf("\tJOB status = [%d]\t  JOB return = [%d]\n ----------------------\n\n", job->status, job->ret);
	/*		*/
	return (0);
}

