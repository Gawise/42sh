#include "libft.h"
#include "exec.h"
#include "struct.h"
#include "sh.h"
#include "var.h"

int			unsetenv_find(void *elem, void *var)
 {
	 if (ft_strcmp(((t_var *)elem)->ctab[0], (char *)var) == 0)
        return (1);
    return (0);
}

void		unsetenv_del(void *delete, size_t size)
{
	(void)size; //pk avoir rajouter the fcking size ?
    ft_del_tab(((void **)((t_var *)delete)->ctab));
    free(delete);
}

uint8_t			ft_unsetenv(t_job *j, t_process *p)
{
	int		i;

	(void)j; //useles job pour tout le monde ????????????
	i = 0;
	while (p->av[++i])
		ft_lstdelif(&cfg_shell()->env, p->av[i], unsetenv_find, unsetenv_del);
	return (SUCCESS);
}
