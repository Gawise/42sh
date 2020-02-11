#include "libft.h"
#include "exec.h"


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
	ft_lst_push_back(lst, &new, sizeof(new));
	return (0);
}

int			ft_setenv(t_list **lst, char *var, char *value, uint8_t overw)
{
	t_list *find;

	find = NULL;
	if (ft_strchr(var, '=') || ft_strchr(value, '='))
		return (1);
	if ((find = setenv_find(*lst, var)))
	{
		if (overw)
			setenv_update(find, value);
		return (0);
	}
	return (setenv_add(lst, var, value));
}


/*
 *                 useles ? if not, have to update
 *int			ft_call_setenv(t_msh *msh)
 *{
 *    char	*value;
 *    char	*var;
 *
 *    if (!msh->input[1])
 *        return (0);
 *    if ((value = ft_strchr(msh->input[1], '=')))
 *    {
 *        *value = '\0';
 *        value++;
 *    }
 *    var = msh->input[1];
 *    return (ft_setenv(&msh->env_var, var, value, 1));
 *}
 */
