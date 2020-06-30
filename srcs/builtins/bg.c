#include "ft_printf.h"
#include "struct.h"
#include "sh.h"
#include "job_control.h"
#include "exec.h"
#include <signal.h>


# define BG_USG "bg: usage: bg [job_spec ...]"


uint8_t		bg_opt(uint8_t interactive, t_process *p, int32_t *ac)
{
	int32_t		i;
	int8_t		ret;

	i = 0;
	if (!interactive)
	{
		ft_dprintf(STDERR_FILENO, "bg: no job control\n");
		return (FAILURE);
	}
	while ((ret = ft_getopt(ac, &i, p->av, "")) != -1)
	{
		if (ret == '?') //useless ? -- ?
			ft_dprintf(STDERR_FILENO, "%s: bg: %c: invalide option\n%s\n", PROJECT, p->av[*ac][i], BG_USG);
		return (FAILURE);
	}
	return (SUCCESS);
}


uint8_t		put_job_in_bg(t_cfg *shell, char *wanted)
{
	t_job	*target;

	target = 0;
	if (find_target(shell->job, wanted, &target, "bg") == FAILURE)
		return (FAILURE);
	if (target->status & RUNNING) //running ou bg ?
	{
		ft_dprintf(STDERR_FILENO, "%s: bg: job %d already in background\n", PROJECT, target->id);
		return (SUCCESS);
	}

	job_is_running(target);
	target->fg = 0;

	kill(-target->pgid, SIGCONT);

	set_job_background(target);
	target->pgid = 0;
	ft_lstdelif(&shell->job, &target->pgid, focus_job, del_struct_job);
	
	/*
	jcpy = malloc(sizeof(t_job));
	protect_malloc(jcpy);
	ft_cpy_job(target, jcpy); //protct malloc ?
	ft_lstdelif(&shell->job, &target->pgid, focus_job, del_struct_job);
	target = 0; // protect de dev
	*/
	return (SUCCESS);
}


uint8_t		ft_bg(t_job *j, t_process *p)
{
	(void)j;
	t_cfg		*shell;
	int32_t		ac;
	uint8_t		err;

	shell = cfg_shell();
	ac = 1;
	err = 0;
	if (bg_opt(shell->interactive, p, &ac) == FAILURE)
		return (2);
	if (!(p->av[ac]))
		err = put_job_in_bg(shell, 0);
	else
	{
		while (p->av[ac])
		{
			err += put_job_in_bg(shell, p->av[ac]);
			ac++;
		}
	}




	return (SUCCESS);
}
