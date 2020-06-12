#include "libft.h"
#include "struct.h"
#include "var.h"
#include "sh.h"
#include "ft_printf.h"
#include "exec.h"
#include "job_control.h"

t_list			*find_job_by_status(t_list *lst, uint8_t want)
{
	t_job	*j;

	while (lst)
	{
		j = lst->data;
		if (j->status == want)
			return (lst);
		lst = lst->next;
	}
	return (NULL);
}

void			add_job_cfg(t_job *job)
{
	t_cfg	*shell;
	t_job	jc;

	shell = cfg_shell();
	shell->active_job++;
	job->id = shell->active_job;
	ft_cpy_job(job, &jc);
	ft_lst_push_back(&shell->job, &jc, sizeof(t_job));
	ft_bzero(&jc, sizeof(t_job));
	debug_print_job(shell->debug, &jc, "add_job_cfg");
}

void			set_job_background(t_job *job)
{
	add_job_cfg(job);
	ft_printf("[%d] %d\n", job->id, job->pgid);
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

void			ft_cpy_job(t_job *job, t_job *copy)
{
	ft_bzero(copy, sizeof(t_job));
	copy->cmd = ft_strdup(job->cmd);
	copy->process =
		ft_lstdup(job->process, job->process->size, cpy_lst_process);
	copy->pgid = job->pgid;
	copy->fg = job->fg;
	copy->status = job->status;
	copy->ret = job->ret;
	copy->id = job->id;
	ft_memset(copy->std, -1, sizeof(int16_t) * 3);
	ft_memcpy(&copy->term_eval, &job->term_eval, sizeof(job->term_eval));
}
