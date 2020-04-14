#include "libft.h"
#include "struct.h"
#include "var.h"
#include "sh.h"


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
	ft_memcpy(&copy->term_eval, &job->term_eval, sizeof(job->term_eval));
}
