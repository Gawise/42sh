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
		ft_dprintf(STDERR_FILENO,"have job running or stopped ! \n");
		protect_job++;
		return (FAILURE);
	}
	ret = (p->av[1]) ? ft_atoi(p->av[1]) :
		ft_atoi(find_var_value(shell->intern, "?"));
	ft_printf("exit\n");
	exit(ret);
}
