#include "libft.h"
#include "exec.h"
#include "struct.h"
#include "sh.h"
#include "var.h"
#include "ft_printf.h"
#include "job_control.h"
#include <sys/types.h>
#include <sys/wait.h>

uint8_t		print_bg_error(char *s, uint8_t jid)
{
	if (!jid)
	{
		ft_dprintf(STDERR_FILENO, "21sh: bg: %s: no such job\n", s);
		return (1);
	}
	ft_dprintf(STDERR_FILENO, "21sh: bg: job %d already in background\n", jid);
	return (0);
}

int16_t		get_bg_first_id(char *str)
{
	int16_t		jid;

	if (!str)
		jid = get_job_id("+");
	else if (!ft_strcmp(str, "--"))
		return (-2);
	else if (str[0] == '-' && str[1])
	{
		ft_dprintf(STDERR_FILENO, "21sh: bg: -%c: invalid option\n", str[1]);
		ft_dprintf(STDERR_FILENO, "bg : usage bg [job_spec]\n");
		return (-1);
	}
	else
		jid = get_job_id(str);
	return (jid);
}

uint8_t		put_job_in_bg(t_job *j, char *ope, uint8_t jid)
{
	t_cfg		*shell;
	t_list		*job;

	shell = cfg_shell();
	job = cfg_shell()->job;
	if (!jid || !job)
		return (print_bg_error(ope, 0));
	while (job)
	{
		j = job->data;
		if (j->id == jid)
			break;
		if (!job->next)
			return (print_bg_error(ope, 0));
		job = job->next;
	}
	if (j->status == RUNNING)
		return (print_bg_error(ope, jid));
	ft_printf("[%d] %s &\n", j->id, j->cmd);
	job_is_running(j);
	set_termios(TCSADRAIN, &shell->term_origin);
	kill(-j->pgid, SIGCONT);
	update_listjob(shell);
	return (0);
}

int		bg_curr_job(t_job *j, int16_t jid)
{
	t_list 	*job;

	job = cfg_shell()->job;
	if (!job)
	{
		ft_dprintf(STDERR_FILENO, "21sh: bg: current: no such job\n");
		return (0);
	}
	else 
	{
		jid = get_job_id("+");
		put_job_in_bg(j, ft_itoa(jid), jid);
	}
	return (1);
}

uint8_t		ft_bg(t_job *j, t_process *p)
{
	int16_t		jid;
	uint8_t		i;
	int8_t		ret;

	ret = 0;
	i = 1;
	jid = 0;
	if (!p->av[1] && !bg_curr_job(j, jid))
		return (FAILURE); // ??
	while (p->av[i])
	{
		if (i == 1)
		{
			if ((jid = get_bg_first_id(p->av[1])) == -1)
				return (2);// erreur options.
			else if (jid >= 0)
				ret += put_job_in_bg(j, p->av[i], jid);
		}
		else
		{
			jid = get_job_id(p->av[i]);
			ret += put_job_in_bg(j, p->av[i], jid);
		}
		i++;
	}
	return (ret ? 1 : 0);
}
// si bg a un job fini, message special "job has terminated", je renvoie "job already in bg"
