#include "libft.h"
#include "exec.h"
#include "sh.h"
#include "ft_printf.h"

uint8_t		ft_print_env_lst(t_list *lst)
{
	t_var	*env;

	while (lst)
	{
		env = lst->data;
		ft_printf("%s=%s\n", env->ctab[0], env->ctab[1]);
		lst = lst->next;
	}
	return (SUCCESS);
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
			ft_ex(EXMALLOC);
		tab[count] = NULL;
	}
	count--;
	var = lst->data;
	ft_asprintf(&tab[count], "%s=%s", var->ctab[0], var->ctab[1]);
	return (tab);
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
		ft_lst_push_back(lst, &var, sizeof(t_var));
		i++;
	}
}
