#include "libft.h"
#include "struct.h"
#include "sh.h"
#include "ft_printf.h"
#include "var.h"

char		**ret_empty_tab(void)
{
	char **res;

	if (!(res = ft_memalloc(sizeof(char *))))
		ft_ex(EXMALLOC);
	res[0] = NULL;
	return (res);
}

char		**retrieve_ops(char **p_av, int *i, int *bang)
{
	char	**res;
	int	brack;

	brack = 0;
	*i = 1;
	if (!p_av[1])
		return (ret_empty_tab());
	if (p_av[0] && ft_strequ("[", p_av[0]))
		brack = 1;
	while (p_av[*i])
		*i += 1;
	if (p_av[1] && ft_strequ(p_av[1], "!"))
		*bang = 1;
	if (!(res = ft_memalloc(sizeof(char *) * (*i - brack - *bang))))
		return (NULL);
	*i = 1 + *bang;
	while (p_av[*i + brack])
	{
		if (!(res[*i - 1 - *bang] = ft_strdup(p_av[*i])))
			ft_ex(EXMALLOC);
		*i += 1;
	}
	res[*i - 1 - *bang] = NULL;
	*i -= (1 + *bang);
	return (res);
}

int		check_closing_bracket(char **av)
{
	int	i;

	i = 0;
	while (av[i] && av[i + 1])
		i++;
	if (ft_strequ(av[i], "]"))
		return (0);
	return (1);
}
