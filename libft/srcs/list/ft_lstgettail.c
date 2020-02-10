#include "libft.h"

t_list	*ft_lstgettail(t_list **alst)
{
	t_list	*item;

	if (!alst || !*alst)
		return (NULL);
	item = *alst;
	while (item->next)
		item = item->next;
	return (item);
}
