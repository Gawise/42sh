#include "exec.h"
#include "libft.h"


#include "sh.h"
#include "ft_printf.h"
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

void	routine_clean_job(void *del, size_t u)
{
	t_job	*j;

	j = del;
	ft_strdel(&j->cmd);
	ft_lstdel(&j->process, del_struct_process);
	close(j->std[0]);
	close(j->std[1]);
	close(j->std[2]);
	ft_bzero(j, u);
}

void	del_struct_job(void *del, size_t u)
{
	routine_clean_job(del, u);
	free(del);
}

void	clean_cfg(t_cfg *shell)
{
	ft_lstdel(&shell->env, del_struct_tvar);
	ft_lstdel(&shell->intern, del_struct_tvar);
	ft_lstdel(&shell->sp, del_struct_tvar);
	ft_lstdel(&shell->job, del_struct_job);
	ft_dlstdel(&shell->history);
	ft_hash_del_map(&shell->map, free);
	ft_hash_del_map(&shell->input_map, free);
	if (shell->debug > 2)
		close(shell->debug);
	ft_bzero(shell, sizeof(t_cfg));
}
