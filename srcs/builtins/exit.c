#include "libft.h"
#include "struct.h"
#include "sh.h"
#include "ft_printf.h"
#include "var.h"


uint8_t		ft_exit(t_job *j, t_process *p)
{
	static int	protect_job = 0;
	uint8_t 		ret;
	t_cfg		*shell;

	(void)j;
	shell = cfg_shell();
	if (shell->job && !protect_job)
	{
		ft_dprintf(STDERR_FILENO,"have job running or stopped !\n");
		protect_job++;
		return (FAILURE);
	}
	ret = (p->av[1]) ? ft_atoi(p->av[1]) :
		ft_atoi(find_var_value(shell->intern, "?"));
	clean_cfg(shell);
	ft_printf("exit\n");
	exit(ret);
	/* The shell exits by default upon receipt of a SIGHUP. Before exiting, an interactive shell resends the SIGHUP to all jobs, running or stopped. Stopped jobs are sent SIGCONT to ensure that they receive the SIGHUP. */
}
