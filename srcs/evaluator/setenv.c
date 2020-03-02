#include "libft.h"
#include "var.h"
#include "struct.h"
#include "sh.h"


static void	setenv_update(t_list *lst, char *value)
{
	ft_strdel(&(((t_var *)(lst->data))->ctab[1]));
	((t_var *)(lst->data))->ctab[1] = ft_strdup(value);
}

static int	setenv_add(t_list **lst, char *var, char *value)
{
	t_var	new;

	if (!(new.ctab = (char **)malloc(sizeof(char *) * 3)))
		ex("ERROR Malloc");
	new.ctab[0] = ft_strdup(var);
	new.ctab[1] = ft_strdup(value);
	new.ctab[2] = NULL;
	new.rd = 0;
	ft_lst_push_back(lst, &new, sizeof(new));
	return (0);
}

int			ft_setvar(t_list **lst, char *name, char *value, uint8_t overw)
{
	t_list *find;
	t_var	*var;

	find = NULL;
	if (ft_strchr(name, '=') || ft_strchr(value, '='))
		return (1);
	if ((find = find_var(*lst, name)))
	{
		var = find->data;
		if (overw && !var->rd)
			setenv_update(find, value);
		return (0);
	}
	return (setenv_add(lst, name, value));
}

int			ft_setenv(t_list **lst, char *name, char *value, uint8_t overw)
{
	return (ft_setvar(lst, name, value, overw));
}
