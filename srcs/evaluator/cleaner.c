#include "exec.h"
#include "libft.h"

#include <stdio.h>

void	del_struct_process(void *del, size_t u)
{
	t_process *p;

	(void)u;
	p = del;
	ft_strdel(&(p->cmd));
	ft_del_tab((void **)p->av);
	ft_strdel(&p->path);
	free(del);
}

void	del_struct_tvar(void *del, size_t u)
{

	t_var	*v;

	(void)u;
	v = del;
	ft_del_tab((void **)v->ctab);
	free(v);
}

int		routine_clean_job(t_job *j)
{
	ft_strdel(&j->command);
	ft_lstdel(&j->process, del_struct_process);
	ft_lstdel(&j->env, del_struct_tvar);
	return (0);
}
