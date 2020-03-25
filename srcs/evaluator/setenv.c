#include "libft.h"
#include "exec.h"
#include "struct.h"
#include "sh.h"
#include "var.h"
#include "ft_printf.h"


static void	setenv_update(t_list *lst, char *value)
{
	ft_strdel(&(((t_var *)(lst->data))->ctab[1]));
	((t_var *)(lst->data))->ctab[1] = ft_strdup(value);
}

static int	setenv_add(t_list **lst, char *var, char *value)
{
	t_var	new;

	if (!(new.ctab = (char **)malloc(sizeof(char *) * 3)))
		ft_ex("[Fatal Error] MALLOC\nexit\n");
	new.ctab[0] = ft_strdup(var);
	new.ctab[1] = ft_strdup(value);
	new.ctab[2] = NULL;
	ft_lst_push_back(lst, &new, sizeof(new));
	return (SUCCESS);
}


uint8_t		var_check_name(char *name)
{
	if (*name > 47 && *name < 58)
		return (FAILURE);
	while (*name)
	{
		if (*name != '_' && !ft_isalnum(*name))
			return (FAILURE);
		name++;
	}
	return (SUCCESS);
}

int			ft_setvar(t_list **lst, char *name, char *value)
{
	t_list *find;
	t_var	*var;

	find = NULL;
	if (var_check_name(name))
		return (FAILURE);
	if ((find = find_var(*lst, name)))
	{
		var = find->data;
		setenv_update(find, value);
		return (SUCCESS);
	}
	return (setenv_add(lst, name, value));
}

static uint8_t		setenv_check_av(char **av)
{
	uint8_t		i;
	int8_t		j;

	i = 0;
	while (av[i])
	{
		j = 0;
		if (((j = ft_strchri(av[i], '=')) != -1) &&
				(j == 0 || !av[i][j + 1]))
		{
			ft_dprintf(STDERR_FILENO, "'%s': Not a valide identifier\n", av[i]);
			return (FAILURE);
		}
		i++;
	}
	return (SUCCESS);
}

uint8_t			ft_setenv(t_job *j, t_process *p)
{
	char	*value;
	char	*tmp;
	int		i;

	(void)j; //useles job pour tout le monde ????????????
	i = 1;
	if (setenv_check_av(&p->av[i]) == FAILURE)
		return (FAILURE);
	while (p->av[i])
	{
		value = NULL;
		if ((tmp = ft_strchr(p->av[i], '=')) && (value = tmp +1))
			*tmp = '\0';
		if (ft_setvar(&cfg_shell()->env, p->av[i], value) == FAILURE)
			ft_dprintf(STDERR_FILENO, "'%s': Not a valide identifier\n", p->av[i]);
		if (tmp)
			*tmp = '=';
		i++;
	}
	return (SUCCESS);
}
