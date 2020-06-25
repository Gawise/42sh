#include "libft.h"
#include "ft_printf.h"
#include "sh.h"

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

uint8_t		ft_fg(t_job *j, t_process *p)
{
	t_cfg	*shell;
	int32_t		ac;

	(void)j;
	shell = cfg_shell();
	ac = 1;
	if (fg_opt(shell->interactive, p, &ac) != SUCCESS)
		return (2);


	return (1);
}
