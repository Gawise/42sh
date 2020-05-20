#include "libft.h"
#include "exec.h"

t_process	*find_process_by_pid(t_list *lst, pid_t child)
{
	t_process *p;

	while (lst)
	{
		p = lst->data;
		if (p->pid == child)
			return (p);
		lst = lst->next;
	}
	return (NULL);
}

t_process	*find_process_by_status(t_list *lst, uint8_t want)
{
	t_process *p;

	while (lst)
	{
		p = lst->data;
		if (p->status & want)
			return (p);
		lst = lst->next;
	}
	return (NULL);
}

void		process_type(t_process *p)
{
	uint32_t	err;

	err = 0;
	if (ft_strchr(p->cmd, '/'))
		with_slash(p, &err);
	else (any_slash(p->env, p, &err));
	if (err && (p->setup & R_ERROR))
		p->setup |= P_ERROR;
	else if (err)
		p->setup |= process_errors_handling(p, err);
}
