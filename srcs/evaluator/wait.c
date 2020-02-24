#include "exec.h"
#include "libft.h"
#include <sys/types.h>
#include <sys/wait.h>

void	call_jobcontroler(t_job *j)
{
	(void)j;
	printf("[nb job] Stopped(SIGTSTP)  ->commande line used for message\n");
}

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

int		aplylyse_wstatus(t_process *p, int wstatus)
{
	// pour sigint ou sigtstp definir directement dans job ou parcourir la list pour update
	// les deux ? 
	if (WIFEXITED(wstatus))
	{
		p->status = COMPLETED;
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
	return (p->status);
}

int		update_process(t_list *lst, pid_t child, int wstatus)
{
	t_process *p;

	p = find_process_by_pid(lst, child);
	if (!(p))                          //debug
		ex("[Update Process] PID Non Trouve");
	return (aplylyse_wstatus(p, wstatus));
}

void		update_job(t_job *j)
{
	t_list		*lst;

	if (j->status == COMPLETED)
	{
		if (!(lst = ft_lstgettail(j->process)))
			return ; // en attendant qu un job ne peut etre lancer vide
		j->retour = ((t_process *)(lst->data))->retour;
	}
	if (j->status == KILLED)
		j->retour = 130;
	if (j->status == STOPPED)
		call_jobcontroler(j);
}

int		wait_process(t_job *job)
{
	pid_t		pid_child;
	int			wstatus;

	printf("\t[WAIT PROCESS]\t pgid = [%d]\n", job->pgid);
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
		printf("\n\n --> INFO JOB before leave\n");
		while (j)
		{
			process = j->data;
			printf("cmd = [%s]\t retour = [%d]\t status = [%d]\n", process->path, process->retour, process->status);
			j = j->next;
		}
		printf("JOB status = [%d]\t  JOB return= [%d]\n", job->status, job->retour);
	/*		*/
	printf("sort du wait\n");
	return (0);
}

