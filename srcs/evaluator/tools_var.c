#include "libft.h"
#include "exec.h"
#include "sh.h"




void	cpy_var_list(void *dst, void *src)
{
	t_var *d;
	t_var *s;

	d = dst;
	s = src;
	d->ctab = ft_tabdup(s->ctab);
	d->rd = s->rd;
}

char		**create_tab_var(t_list *lst, int count)
{
	char	**tab;
	t_var	*var;

	if (!lst)
		return (NULL);
	if (lst->next)
		tab = create_tab_var(lst->next, ++count);
	if (!lst->next)
	{
		count++;
		if (!(tab = (char **)malloc(sizeof(char *) * (count + 1))))
			ft_ex("[Fatal Error] MALLOC\nexit\n");
		tab[count] = NULL;
	}
	count--;
	/*    plus besoin si strjoin valide avec stdarg
	 *len0 = ft_strlen(((t_var *)(lst->data))->ctab[0]);
	 *len1 = ft_strlen(((t_var *)(lst->data))->ctab[1]);
	 *ctab[count] = ft_create_var_env(((t_var *)(lst->data))->ctab, len0, len1);
	 */
	var = lst->data;
	tab[count] = ft_strjoin(3, var->ctab[0], "=", var->ctab[1]);
	return (tab);
}

t_list		*find_var(t_list *lst, char *name)
{
	t_var *var;

	while (lst)
	{
		var = lst->data;
		if (!ft_strcmp(var->ctab[0], name))
			return (lst);
		lst = lst->next;
	}
	return (NULL);
}

char		*find_var_value(t_list *var, char *name)
{
	t_list	*lst;

	lst = find_var(var, name);
	if (lst)
		return (((t_var *)(lst->data))->ctab[1]);
	return (NULL);
}

void		create_lst_var(t_list **lst, char **tab)
{
	int		i;
	t_var	var;

	i = 0;
	*lst = NULL;
	if (!tab || !*tab)
		return ;
	while (tab[i])
	{
		var.ctab = ft_strsplit(tab[i], "=");
		var.rd = 0;
		ft_lst_push_back(lst, &var, sizeof(t_var));
		i++;
	}
}
