#include "libft.h"
#include "lexer.h"
#include "struct.h"
#include "exec.h"
#include "sh.h"
#include "ft_printf.h"
#include "var.h"

/*
 *	- on ajoute "test" et "[" a la hash_table des builtins
 *	- une hash_table pour les opreateurs ? initialisee dans init_shell ?
 *	- return	pas de $1 => exit(0)
 *			1 arg si $1 != NULL => exit(1)
 */ 

static uint8_t		bin_op(char *cmd, char **av, int bang)
{
	uint8_t		(*fct)(char *, char *);
	uint8_t		res;

	if (!(fct = ft_hash_lookup(cfg_shell()->test_bin, av[1])))
	{
		ft_dprintf(2, "%s: %s: %s: binary operator expected\n",
				PROJECT, cmd, av[1]);
		return (2);
	}
	res = fct(av[0], av[2]);
	if ((bang && !res) || (!bang && res))
		return (1);
	return (0);
}

static uint8_t		un_op(char *cmd, char **av, int bang)
{
	uint8_t		(*fct)(char *);
	uint8_t		res;

	if (!(fct = ft_hash_lookup(cfg_shell()->test_un, av[0])))
	{
		ft_dprintf(2, "%s: %s: %s: unary operator expected\n",
				PROJECT, cmd, av[0]);
		return (2);
	}
	res = fct(av[1]);
	if ((bang && !res) || (!bang && res))
		return (1);
	return (0);
}

static uint8_t		single_str(char **av, int bang)
{
	uint8_t		res;

	res = 1;
	if (av[0] && av[0][0])
		res = 0;
	if ((bang && !res) || (!bang && res))
		return (1);
	return (0);
}

uint8_t		ft_test(t_job *j, t_process *p)
{
	uint8_t		ret;
	char		**av;
	int		count;
	int		bang;

	(void)j;
	bang = 0;
	if (ft_strequ(p->cmd, "[") && !check_closing_bracket(p->av))
		return (2);
	if (!(av = retrieve_ops(p->av, &count, &bang)))
		return (2);
	if (count >= 4)
	{
		ft_dprintf(2, "%s: %s: Too many arguments\n", PROJECT, p->cmd);
		return (2);
	}
	else if (count == 3)
		ret = bin_op(p->cmd, av, bang);
	else if (count == 2)
		ret = un_op(p->cmd, av, bang);
	else
		ret = single_str(av, bang);
	tabfree(av);
	return (ret);
}
