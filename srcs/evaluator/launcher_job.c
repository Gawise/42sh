#include "parser.h"
#include "exec.h"
#include "libft.h"
#include "var.h"
#include "sh.h"
#include "job_control.h"
#include <unistd.h>

#include <termios.h>

#include <stdlib.h>

#include "ft_printf.h"
#include "sh.h"




void		routine_process(t_cfg *shell, t_list *process, t_pipe *fd)
{
	t_process	*manage;
	t_list		*env;

	env = shell->env;
	manage = process->data;
	if (env)
		manage->env = ft_lstdup(env, env->size, cpy_var_list);
	fd->tmp = fd->fd[0];
	fd->fd[0] = 0;				/* set a 0, voir si pas mieux a -1*/
	fd->fd[1] = 0;
	if (!process->next)
		return ;
	if (pipe(fd->fd) == -1)
		ft_ex("pipe routine process"); // debug
	manage->std[1] = fd->fd[1];
	manage->setup |= PIPE_ON;
	manage = process->next->data;
	manage->std[0] = fd->fd[0];
	manage->setup |= PIPE_ON;
}

uint8_t		routine_ending_job(t_cfg *shell, t_job *job)
{
	char	*ret;

	if (!shell->interactive)
		wait_process(job);
	else if (job->fg)
	{
	/*	tcsetpgrp(STDIN_FILENO, job->pgid) // seulememt ici du coup ?? */
		wait_process(job);
		tcsetpgrp(STDIN_FILENO, shell->pid);
		set_termios(TCSADRAIN, &shell->term_origin);
	}
	else
		set_job_background(job);
	ret = ft_itoa(job->ret);
	setvar_update(find_var(shell->sp, "?"), ret);
	ft_strdel(&ret);
	return (job->ret);
}

uint8_t		run_job(t_cfg *shell, t_job *job, t_list *process)
{
	job->status |= RUNNING;
	ft_lstiter(job->process, job_redir);
	while (process)
	{
		routine_process(shell, process, &job->pipe);
		run_process(shell, job, process->data);
		process = process->next;
		if (job->pipe.tmp)
			if (close(job->pipe.tmp) == -1)
				ft_ex("[check and do pipe] close error:"); //debug
	}
	return (routine_ending_job(shell, job));
}
