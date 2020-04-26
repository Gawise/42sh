#include "sh.h"
#include "struct.h"
#include "exec.h"
#include "job_control.h"
#include "ft_printf.h"
#include <sys/types.h>
#include <sys/wait.h>

static void		one_process_change(t_process *p)
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

static void		job_done(t_job *j, t_process *last_p)
{
	j->status = last_p->status;
	j->ret = last_p->ret;

	if (last_p->status & (FAILED | COMPLETED))
		ft_printf("\n[%d]\tDone(%d)\t%s\n", j->id, j->ret, j->cmd);
	else if (last_p->status & KILLED)
	{
		j->ret += 128;
		if (last_p->ret == 3)
			ft_printf("\n[%d]\tQuit(SIGQUIT)(%d)\t%s\n", j->id, j->ret, j->cmd);
		else
			ft_printf("\n[%d]\tKilled(SIGKILL)(%d)\t%s\n", j->id, j->ret, j->cmd);
	}
}

static uint8_t	deep_check(t_job *j, pid_t child, int32_t wstatus)
{
	pid_t	last_child;


	ft_dprintf(cfg_shell()->debug, "[DEEP CHECK]1  RENTRE\n child = [%d]  wstatus = [%d]\n", child , wstatus);
	last_child = child;
	update_process(j->process, child, wstatus);
	child = waitpid(-j->pgid, &wstatus, WUNTRACED | WNOHANG);
	ft_dprintf(cfg_shell()->debug, "[DEEP CHECK]2\n child = [%d]  wstatus = [%d]\n", child , wstatus);
	if (child == -1)
		job_done(j, find_process_by_pid(j->process, last_child));
	else if (child > 0)
		deep_check(j, child, wstatus);
	else
		one_process_change(find_process_by_pid(j->process, last_child));
	return (TRUE);
}

static void		update_listjob(t_cfg *shell)
{
	uint8_t		ending_status;
	t_list		*ljob;
	t_job		*j;

	ending_status = (FAILED | COMPLETED | KILLED);
	while (ft_lstdelif(&shell->job, &ending_status, job_has_finish, del_struct_job) == SUCCESS)
		;
	ljob = shell->job;

	ft_dprintf(shell->debug, "[UPDATE LISTJOB] RENTRE\n");
	if (!ljob)
		shell->active_job = 0;
	while (ljob)
	{
		ft_dprintf(shell->debug, "[UPDATE LISTJOB] boucle\n");
		j = ljob->data;
		if (find_process_by_status(j->process, RUNNING))
			j->status = RUNNING;
		else if (find_process_by_status(j->process, STOPPED))
			j->status = STOPPED;
		else
			ft_dprintf(shell->debug, "job has lost [%d] [%s]\n",j->pgid, j->cmd); //debug
		if (!ljob->next)
			shell->active_job = j->id;
		ljob = ljob->next;
	}
}

void			check_child(t_cfg *shell, t_list *lstjob)
{
	pid_t		pid_child;
	t_job		*job;
	int32_t		wstatus;
	uint8_t		new;

	new = 0;
	ft_dprintf(shell->debug, "[CHECK_CHILD] RENTRE\n ");
	if (!shell->active_job)
		return ;
	while (lstjob)
	{
		ft_dprintf(shell->debug, "[CHECK CHILD] boucle\n");
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
			job = job_debug->data;
			debug_print_all(job, job->process, "check_child");
			job_debug = job_debug->next;
		}
	}
}
