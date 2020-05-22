#include "exec.h"
#include "libft.h"
#include "sh.h"
#include "ft_printf.h"
#include "job_control.h"
#include <sys/types.h>
#include <sys/wait.h>



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


char		**create_message_signal(char **tab)
{
	ft_bzero(tab, sizeof(char *) * 28);
	tab[1] = S_SIGHUP;
	tab[2] = S_SIGINT;
	tab[3] = S_SIGQUIT;
	tab[4] = S_SIGILL;
	tab[5] = S_SIGTRAP;
	tab[6] = S_SIGABRT;
	tab[7] = S_SIGBUS;
	tab[8] = S_SIGFPE;
	tab[9] = S_SIGKILL;
	tab[10] = S_SIGUSR1;
	tab[11] = S_SIGSEGV;
	tab[12] = S_SIGUSR2;
	tab[14] = S_SIGALRM;
	tab[15] = S_SIGTERM;
	tab[19] = S_SIGSTOP;
	tab[20] = S_SIGTSTP;
	tab[21] = S_SIGTTIN;
	tab[22] = S_SIGTTOU;
	tab[24] = S_SIGXCPU;
	tab[25] = S_SIGXFSZ;
	tab[26] = S_SIGALRM;
	tab[27] = S_SIGPROF;
	return (tab);
}

uint8_t		print_message_signal(uint8_t sig, t_job *j)
{
	char	*tab[28];

	create_message_signal(tab);
	if ((sig < 19 || sig > 22) && tab[sig] && j->fg)
		ft_printf("%s\n", tab[sig]);
	else if (tab[sig])
		ft_printf("\n[%d]\t+ %s  %s\n", j->id, tab[sig], j->cmd);
	return (sig + 128);
}

void	call_jobcontroler(t_job *j)
{
	j->status = STOPPED;
	add_job_cfg(j);
	print_message_signal(j->ret - 128, j);
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
	else
	{
		lst = ft_lstgettail(j->process);
		tmp = lst->data;
		j->status = ((t_process *)(lst->data))->status;
		if (j->status == KILLED)
			j->ret = print_message_signal(tmp->ret, j);
		else
			j->ret = tmp->ret;
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
