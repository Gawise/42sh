#include "libft.h"
#include "exec.h"
#include "struct.h"
#include "sh.h"
#include "var.h"
#include "ft_printf.h"

static uint8_t		setenv_check_av(char **av)
{
	uint8_t		i;
	int8_t		j;

	i = 0;
	while (av[i])
	{
		j = 0;
		if (((j = ft_strchri(av[i], '=')) != -1) &&
				(j == 0 || !av[i][j + 1]))
		{
			ft_dprintf(STDERR_FILENO, "'%s': Not a valide identifier\n", av[i]);
			return (FAILURE);
		}
		i++;
	}
	return (SUCCESS);
}

uint8_t			ft_setenv(t_job *j, t_process *p)
{
	int		i;
	t_list	**env;

	(void)j; //useles job pour tout le monde ????????????
	i = 1;
	env = &cfg_shell()->env;
	if (setenv_check_av(&p->av[i]) == FAILURE)
		return (FAILURE);
	while (p->av[i])
	{
		if (several_setvar(env, p->av[i]) == FAILURE)
			ft_dprintf(STDERR_FILENO, "'%s': Not a valide identifier\n", p->av[i]);
		i++;
	}
	return (SUCCESS);
}
