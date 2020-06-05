#include "libft.h"
#include "struct.h"
#include "sh.h"
#include "ft_printf.h"
#include "var.h"

void		print_args(char **av)
{
	while (*av)
	{
		ft_printf("%s\n", *av);
		av++;
	}
}

int		check_int_args(char *s1, char *s2)
{
	if (!ft_strdigit(s1))
	{
		ft_dprintf(2, "%s: test: %s: integer expression expected\n", PROJECT, s1);
		return (0);
	}
	if (!ft_strdigit(s2))
	{
		ft_dprintf(2, "%s: test: %s: integer expression expected\n", PROJECT, s2);
		return (0);
	}
	return (1);
}

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
	ft_printf("i=%d brack=%d bang=%d\n", *i, brack, *bang);
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
	ft_printf("i=%d brack=%d bang=%d\n", *i, brack, *bang);
	return (res);
}

int		check_closing_bracket(char **av)
{
	int	i;

	i = 0;
	while (av[i] && av[i + 1])
	{
		ft_printf("av[%d] %s\n", i, av[i]);
		i++;
	}
		ft_printf("av[%d] %s\n", i, av[i]);
	if (ft_strequ(av[i], "]"))
		return (1);
	ft_dprintf(2, "%s: [: missing `]'\n");
	return (0);
}
