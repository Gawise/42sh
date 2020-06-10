#include "libft.h"
#include "exec.h"
#include "struct.h"
#include "sh.h"
#include "ft_printf.h"
#include "job_control.h"
#include <sys/wait.h>

void	job_is_running(t_job *j)
{
	t_list		*list;
	t_process	*p;

	list = j->process;
	while (list)
	{
		p = list->data;
		if (p->status == STOPPED)
			p->status = RUNNING;
		list = list->next;
	}
	j->status = RUNNING;
}

uint8_t	print_fg_error(char **s)
{
	if ((!s[1]) || ((!ft_strcmp(s[1], "--")) && (!s[2])))
		ft_dprintf(STDERR_FILENO, "21sh: fg: current: no such job\n");
	else if (!ft_strcmp(s[1], "--"))
		ft_dprintf(STDERR_FILENO, "21sh: fg: %s: no such job\n", s[2]);
	else
		ft_dprintf(STDERR_FILENO, "21sh: fg: %s: no such job\n", s[1]);
	return (1);
}

int16_t	get_fg_jid(t_process *p)
{
	int16_t		jid;

	if (!p->av[1])
		jid = get_job_id("+");
	else if (!ft_strcmp(p->av[1], "--"))
	{
		if (!p->av[2])
			jid = get_job_id("+");
		else
			jid = get_job_id(p->av[2]);
	}
	else if (p->av[1][0] == '-' && p->av[1][1])
	{
		ft_dprintf(STDERR_FILENO, "21sh: fg: -%c", p->av[1][1]);
		ft_dprintf(STDERR_FILENO, ": invalid option\n");
		ft_dprintf(STDERR_FILENO, "fg : usage fg [job_spec]\n");
		return (-1);
	}
	else
		jid = get_job_id(p->av[1]);
	return (jid);
}

int		put_job_in_fg(t_job *j)
{
	t_cfg	*shell;

	shell = cfg_shell();
	ft_printf("%s\n", j->cmd);
	job_is_running(j);
	j->fg = 1;
	if (j->prio)
		update_prio_bg();
	tcsetpgrp(STDIN_FILENO, j->pgid);
	set_termios(TCSADRAIN, &shell->term_origin);
	kill(-j->pgid, SIGCONT);
	wait_process(j);
	update_listjob(shell);
	return (j->ret);
}

uint8_t	ft_fg(t_job *j, t_process *p)
{
	t_list		*job;
	int16_t		jid;

	if (!cfg_shell()->interactive)
	{
		ft_dprintf(STDERR_FILENO, "fg: no job control\n");
		return (1);
	}
	job = cfg_shell()->job;
	if ((jid = get_fg_jid(p)) == -1)
		return (2);
	if (!jid || !job)
		return (print_fg_error(p->av));
	while (job)
	{
		j = job->data;
		if (j->id == jid)
			break ;
		if (!job->next)
			return (print_fg_error(p->av));
		job = job->next;
	}
	return (put_job_in_fg(j));
}
