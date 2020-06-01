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

static uint8_t		bin_op(char **av)
{
	int	i;

	i = 0;
	while (av[i])
		i++;
	ft_printf("test has %d args\n");
}

static uint8_t		un_op(char **av)
{
	int	i;

	i = 0;
	while (av[i])
		i++;
	ft_printf("test has %d args\n");
}

static uint8_t		single_str(char **av)
{
	int	i;

	i = 0;
	while (av[i])
		i++;
	ft_printf("test has %d args\n");
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
		ret = bin_op(av);
	else if (*count == 2)
		ret = un_op(av);
	else
		ret = single_str(av);
	ft_tabfree(av);
	return (0);
}
