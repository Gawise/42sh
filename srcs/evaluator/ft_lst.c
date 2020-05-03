#include "struct.h"
#include "exec.h"
#include "libft.h"

int32_t	has_stopped(t_list *lst)
{
	t_process *p;

	p = lst->data;
	if (p->status & STOPPED)
		return (1);
	return (0);
}

int32_t	has_running(t_list *lst)
{
	t_process *p;

	p = lst->data;
	if (p->status & RUNNING)
		return (1);
	return (0);
}


