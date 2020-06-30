#include "libft.h"
#include "ft_printf.h"
#include "sh.h"
#include "job_control.h"
#include "exec.h"
#include <signal.h>

# define FG_USG "fg: usage: fg [job_spec]"


uint8_t		fg_opt(uint8_t interactive, t_process *p, int32_t *ac)
{
	int32_t		i;
	int8_t		ret;

	i = 0;
	if (!interactive)
	{
		ft_dprintf(STDERR_FILENO, "fg: no job control\n");
		return (FAILURE);
	}
	while ((ret = ft_getopt(ac, &i, p->av, "")) != -1)
	{
		if (ret == '?') //useless ? -- ?
			ft_dprintf(STDERR_FILENO, "%s: fg: %c: invalide option\n%s\n", PROJECT, p->av[*ac][i], FG_USG);
		return (FAILURE);
	}
	return (SUCCESS);
}

uint8_t		find_target(t_list *ljob, char *wanted, t_job **target, char *blt)
{
	uint8_t		curr;
	int8_t		ret;

	ret = 0;
	if (wanted)
	{
		if ((ret = get_job(ljob, wanted, target, &curr)) == FAILURE)
			ft_dprintf(STDERR_FILENO, "%s: %s: %s: no such job\n", PROJECT, blt, wanted);
	}
	else if ((ret = get_job(ljob, "%", target, &curr)) == FAILURE)
			ft_dprintf(STDERR_FILENO, "%s: %s: current: no such job\n", PROJECT, blt);
	return (ret);
}



void	job_is_running(t_job *j)
{
	t_list		*lst;
	t_process	*p;

	lst = j->process;
	while (lst)
	{
		p = lst->data;
		if (p->status & STOPPED)
			p->status = RUNNING;
		lst = lst->next;
	}
	j->status = RUNNING;
}

void		put_job_in_fg(t_cfg *shell, t_job *target)
{
	t_job		*jcpy;
	uint32_t	tmp;

	jcpy = malloc(sizeof(t_job));
	protect_malloc(jcpy);
	tmp = shell->cur_job;
	job_is_running(target);
	target->fg = TRUE;
	ft_cpy_job(target, jcpy); //protct malloc ?
	ft_lstdelif(&shell->job, &target->pgid, focus_job, del_struct_job);
	target = 0; // protect de dev
	ft_printf("%s\n", jcpy->cmd);
	tcsetpgrp(STDIN_FILENO, jcpy->pgid);
	set_termios(TCSADRAIN, &jcpy->term_eval);
	kill(-jcpy->pgid, SIGCONT);
	routine_fg_job(shell, jcpy);
	shell->cur_job = tmp;
	update_last_return(shell, jcpy->ret);
	if (!(jcpy->status & STOPPED))
		nb_job_active(shell);
	del_struct_job(jcpy, sizeof(t_job));
}

uint8_t		ft_fg(t_job *j, t_process *p)
{
	t_cfg		*shell;
	int32_t		ac;
	t_job		*target;

	(void)j;
	target = 0;
	shell = cfg_shell();
	ac = 1;
	if (fg_opt(shell->interactive, p, &ac) != SUCCESS)
		return (2);
	if (find_target(shell->job, p->av[ac], &target, "fg") != SUCCESS)
		return (1);
	put_job_in_fg(shell, target);

	return (1);
}
