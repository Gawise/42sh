#include "libft.h"
#include "exec.h"
#include "sh.h"
#include "var.h"

int32_t		unsetvar_find(void *elem, void *var)
{
	if (ft_strcmp(((t_var *)elem)->ctab[0], (char *)var) == 0)
		return (1);
	return (0);
}

void		unsetvar_del(void *delete, size_t size)
{
	(void)size;
	ft_del_tab(((void **)((t_var *)delete)->ctab));
	free(delete);
}

uint8_t		ft_unset(t_job *j, t_process *p)
{
	int		i;

	(void)j;
	i = 0;
	while (p->av[++i])
	{
		ft_lstdelif(&cfg_shell()->env, p->av[i], unsetvar_find, unsetvar_del);
		ft_lstdelif(&cfg_shell()->intern, p->av[i], unsetvar_find, unsetvar_del);

	}
	return (SUCCESS);
}
