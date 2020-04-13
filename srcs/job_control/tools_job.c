#include "libft.h"
#include "struct.h"
#include "var.h"
#include "sh.h"


void	cpy_lst_process(void *copy, void *process)
{
	t_process *p;
	t_process *c;
//	int8_t *std;

	p = process;
	c = copy;
/*	if (!(std = malloc(sizeof(int8_t) * 3)))
		ft_ex("[Fatal Error] Cannot allocate memory\n"); */
	ft_memcpy(c, p, sizeof(t_process));
	c->cmd = ft_strdup(p->cmd);
	c->av = ft_tabdup(p->av);
	c->path = ft_strdup(p->path);
	c->env = NULL;
	c->assign = NULL;
	c->redir = NULL;
}

void          ft_cpy_job(t_job *job, t_job *copy)
{
	ft_bzero(copy, sizeof(t_job));
	(void)job;
	copy->cmd = ft_strdup(job->cmd);
	copy->process = ft_lstdup(job->process, job->process->size, cpy_lst_process);
	copy->pgid = job->pgid;
	copy->fg = job->fg;
	copy->status = job->status;
	copy->ret = job->ret;
	ft_memcpy(&copy->term_eval, &job->term_eval, sizeof(job->term_eval));
}
