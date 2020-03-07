#include "libft.h"

void	ft_dlstdel(t_dlist **alst)
{
	t_dlist	*tmp;
	t_dlist	*tmp2;

	if (alst && (tmp = *alst))
	{
		while (tmp->prev)
			tmp = tmp->prev;
		while (tmp)
		{
			tmp2 = tmp->next;
			if (tmp2)
				tmp2->prev = NULL;
			ft_memdel((void **)&tmp->data);
			ft_memdel((void **)&tmp);
			tmp = tmp2;
		}
		alst = NULL;
	}
}
