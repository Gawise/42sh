#include "libft.h"

t_list	*ft_lstdup(t_list *lst, size_t size, void (*cplst)(void *, void *))
{
	t_list	*new;

	if (!(lst && lst->data))
		return (NULL);
	new = ft_lstnew(0, 0);
	new->data = ft_memalloc(size);
	new->size = size;
	cplst(new->data, lst->data);
	new->next = ft_lstdup(lst->next, size, cplst);
	return (new);
}
