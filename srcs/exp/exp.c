#include "sh.h"
#include "exp.h"

int	exp_main(char **word, int assign)
{
	t_exp		exp;

	init_exp(&exp);
	if (assign)
	{
		exp.assign = 1;
		find_tilde_exp_assign(word, exp);
	}
	else
	{
		exp.assign = 0;
		find_tilde_exp(word, exp);
	}
	ft_bzero(exp.buf, EXP_BSIZE);
	if (parse_param_exp(word, exp) < 0)
	{
		ft_dprintf(2, "%s: Bad Substitution\n", *word);
		ft_setvar(&cfg_shell()->env, "?", "1", 1);
		return (0);
	}
	return (1);
}
