#include "exec.h"
#include "libft.h"

void	del_struct_tvar(void *del, size_t u)
{

	t_var	*v;

	(void)u;
	v = del;
	ft_del_tab((void **)v->ctab);
	free(v);
}

void	del_struct_process(void *del, size_t u)
{
	t_process *p;

	(void)u;
	p = del;
	ft_lstdel(&p->env, del_struct_tvar);
	ft_strdel(&(p->cmd));
	ft_del_tab((void **)p->av);
	ft_strdel(&p->path);
	free(del);
}

int		routine_clean_job(t_job *j)
{
	ft_strdel(&j->cmd);
	ft_lstdel(&j->process, del_struct_process);
	close(j->std[0]);
	close(j->std[1]);
	close(j->std[2]);
	return (0);
}
