#include "libft.h"
#include "exec.h"

#include <stdio.h>
char		**create_tab_env(t_list *lst, int count)
{
	char	**tab;
	t_var	*env;

	if (!lst)
		return (NULL);
	if (lst->next)
		tab = create_tab_env(lst->next, ++count);
	if (!lst->next)
	{
		count++;
		if (!(tab = (char **)malloc(sizeof(char *) * (count + 1))))
			ex("ERROR MALLOC");
		tab[count] = NULL;
	}
	count--;
	/*    plus besoin si strjoin valide avec stdarg
	 *len0 = ft_strlen(((t_var *)(lst->data))->ctab[0]);
	 *len1 = ft_strlen(((t_var *)(lst->data))->ctab[1]);
	 *ctab[count] = ft_create_var_env(((t_var *)(lst->data))->ctab, len0, len1);
	 */
	env = lst->data;
	tab[count] = ft_strjoin(3, env->ctab[0], "=", env->ctab[1]);
	return (tab);
}

t_list		*setenv_find(t_list *lst, char *name)
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

char		*ft_getenv(t_list *env, char *name)
{
	t_list	*lst;

	lst = setenv_find(env, name);
	if (lst)
		return (((t_var *)(lst->data))->ctab[1]);
	return (NULL);
}

void		create_lst_env(t_list **lst, char **envp)
{
	int		i;
	t_var	env;

	i = 0;
	*lst = NULL;
	if (!envp || !*envp)
		return ;
	while (envp[i])
	{
		env.type = ENV;
		env.ctab = ft_strsplit(envp[i], "=");
		ft_lst_push_back(lst, &env, sizeof(t_var));
		i++;
	}
}

t_list		*env_list_cpy(t_list *lst)
{
	t_list	*cpy;
	char	**envp;

	envp = create_tab_env(lst, 0); //C EST NUL CA !!
	create_lst_env(&cpy, envp);
	ft_del_tab((void **)envp);
	return (cpy);
}
