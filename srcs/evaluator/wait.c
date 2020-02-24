#include "exec.h"
#include "libft.h"
#include <sys/types.h>
#include <sys/wait.h>

int			ft_lsthave(t_list *lst, int (*f)(t_list *elem)) // mettre dans lib
{
	int		nb;

	nb = 0;
	while (lst)
	{
		nb += f(lst);
		lst = lst->next;
	}
	return (nb);
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

int		aplylyse_wstatus(t_job *j, t_process *p, int wstatus)
{
	// pour sigint ou sigtstp definir directement dans job ou parcourir la list pour update
	if (WIFEXITED(wstatus))
	{
		p->status = SUCCES;
		p->retour = WEXITSTATUS(wstatus);
	}
	else if (WIFSIGNALED(wstatus))
	{
		p->status = KILLED; //en attendant de config toutes les possibilites;
		p->retour = WTERMSIG(wstatus);
	}
	else if (WIFSTOPPED(wstatus))
	{
		p->status = STOPPED ; // anakyser en fonction du signal
		p->retour = WSTOPSIG(wstatus);
	}
	return (0);
}

int		update_process(t_list *lst, pid_t child, int wstatus)
{
	t_process *p;

	p = find_process_by_pid(lst, child);
	if (!(p))                          //debug
		ex("[Update Process] PID Non Trouve");
	aplylyse_wstatus(p, wstatus);
	return (0);
}

int		wait_process(t_job *job)
{
	pid_t		pid_child;
	int			wstatus;

	printf("\t[WAIT PROCESS]\t pgid = [%d]\n", job->pgid);
	while (ft_lsthave(job->process, has_running))
	{
		wstatus = 0;
		printf("dans wait\n");
		pid_child = waitpid(-job->pgid, &wstatus, WUNTRACED);
		if (pid_child == -1)
			ex("[WAIT_PROCESS] error waitpid");
		update_process(job, job->process, pid_child, wstatus);
	}


	/* DEBUG  */
		t_process *process;
		t_list *j = job->process;
		printf("\n\n");
		while (j)
		{
			process = j->data;
			printf("cmd = [%s]\t retour = [%d]\t status = [%d]\n", process->path, process->retour, process->status);
			j = j->next;
		}
	/*		*/
	printf("sort du wait\n");
	return (0);
}

