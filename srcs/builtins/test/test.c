#include "libft.h"
#include "struct.h"
#include "sh.h"
#include "ft_printf.h"
#include "var.h"

/*
 *	- on ajoute "test" et "[" a la hash_table des builtins
 *	- une hash_table pour les opreateurs ? initialisee dans init_shell ?
 *	- return	pas de $1 => exit(0)
 *			1 arg si $1 != NULL => exit(1)
 */ 

char		**retrieve_ops(char **p_av, int *i, int *bang)
{
	char	**res;
	int	brack;

	brack = 0;
	*i = 1;
	if (ft_strequ("[", p_av[0]))
		brack = 1;
	while (p_av[*i])
		*i++;
	if (ft_strequ(p_av[1], "!"))
		*bang = 1;
	if (!(res = ft_memalloc(sizeof(char *) * (*i - brack - *bang))))
		return (NULL);
	*i = 1;
	while (p_av[*i + brack + *bang])
	{
		if (!(res[*i - 1] = ft_strdup(p_av[*i])))
			ft_ex(EXMALLOC);
	}
	*i -= 2;
	return (res);
}

int		check_closing_bracket(char **av)
{
	int	i;

	i = 0;
	while (av[i] && av[i + 1])
		i++;
	if (ft_strequ(av[i]), "]")
		return (0);
	return (1);
}

uint8_t		ft_test(t_job *j, t_process *p)
{
	uint8_t		ret;
	char		**av;
	int		count;
	int		bang;

	bang = 0;
	if (ft_strequ(p->cmd, "[") && !check_closing_bracket(p->av))
		return (2);
	if (!(av = retrieve_ops(p->av, *count, *bang)))
		return (2);
	if (*count >= 4)
	{
		ft_dprintf(2, "%s: %s: Too many arguments\n", PROJECT, p->cmd);
		return (2);
	}
	else if (*count == 3)
		// bin op
	else if (*count == 2)
		// un op
	else
		// single string
}
