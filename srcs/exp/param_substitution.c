#include "exp.h"

int	substitute_parameter(t_exp *exp, char **str)
{
	exp_substitute(exp, exp->param);
	*str++;
	return (skip_word(exp, str));
}

int	substitute_word(t_exp *exp, char **str)
{
	str++;
	if (rec_word_parse(exp, str) < 0
	|| exp_main(&exp->word, exp->assign) < 0)
		return (-1);
	exp_substitute(exp, exp->word);
	return (1);
}

int	assign_word(t_exp *exp, char **str, char *param)
{
	if (substitute_word(exp, str) < 0
	|| ft_setvar(&cfg_shell()->env, param, exp->word, 1))
		return (-1);
	return (1);
}

int	substitute_null(t_exp *exp, char **str)
{
	*str++;
	return (skip_word(exp, str));
}

void	substitute_pattern(t_exp *exp, int type)
{
	int	plen;
	int	wlen;

	plen = ft_strlen(exp->param);
	wlen = ft_strlen(exp->word);
	if (wlen > plen)
		exp_substitute(exp, exp->param);
	else if (!type && !ft_strncmp(exp->word, exp->param, wlen))
		exp_substitute(exp, exp->param + wlen);
	else if (type && !ft_strcmp(exp->word, exp->param + plen - wlen))
	{
		*(exp->param + plen - wlen) = '\0';
		exp_substitute(exp, exp->param);
	}
	else
		exp_substitute(exp, exp->param);
}
