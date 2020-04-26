#include "libft.h"
#include "struct.h"
#include "var.h"
#include "sh.h"
#include "exec.h" // only for db
#include "job_control.h"


void		add_job_cfg(t_job *job)
{
	t_cfg	*shell;
	t_job	jc;

	shell = cfg_shell();
	shell->active_job++;
	job->id = shell->active_job;
	ft_cpy_job(job, &jc);
	ft_lst_push_back(&shell->job, &jc, sizeof(t_job));
	ft_bzero(&jc, sizeof(t_job));

	if (shell->debug)
	{
		t_list	*ldb;
		t_job	*jdb;

		ldb = shell->job;
		while (ldb)
		{
			jdb = ldb->data;
			debug_print_all(jdb, jdb->process, "add_job_cfg");
			ldb = ldb->next;
		}
	}
}

static void		cpy_lst_process(void *copy, void *process)
{
	t_process *p;
	t_process *c;

	p = process;
	c = copy;
	ft_bzero(c, sizeof(t_process));
	c->cmd = ft_strdup(p->cmd);
	c->av = ft_tabdup(p->av);
	c->path = ft_strdup(p->path);
	c->status = p->status;
	c->setup = p->setup;
	c->pid = p->pid;
	c->ret = p->ret;
}

void          	ft_cpy_job(t_job *job, t_job *copy)
{
	ft_bzero(copy, sizeof(t_job));
	copy->cmd = ft_strdup(job->cmd);
	copy->process = ft_lstdup(job->process, job->process->size, cpy_lst_process);
	copy->pgid = job->pgid;
	copy->fg = job->fg;
	copy->status = job->status;
	copy->ret = job->ret;
	copy->id = job->id;
	ft_memset(copy->std, -1, sizeof(uint8_t) * 3);
	ft_memcpy(&copy->term_eval, &job->term_eval, sizeof(job->term_eval));
}
