#include "libft.h"

void	ft_lstdelone(t_list **alst, void (*del)(void *, size_t))
{
	del((*alst)->data, (*alst)->size);
	free(*alst);
	*alst = NULL;
}
