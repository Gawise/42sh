#include <sys/types.h>
#include <sys/wait.h>


static void		aplylyse_wstatus(t_job *j, int wstatus)
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

void	what_s_up_child(t_cfg *shell, t_list *lstjob)
{
	t_pid	pid;
	t_job	*job;
	int32_t	wstatus;

	if (!shell->active_job)
		return ;
	while (lstjob)
	{
		job = lstjob->data;
		pid = waitpid(-job->pgid, &wstatus, WHOHAND | WUNTRACED);
		if (pid == -1)
			ft_ex("wait pid what s up child \n");
		if (pid)
			aplylyse_wstatus(t_job *job, wstatus);
		lstjob = lstjob->next;
	}
}
