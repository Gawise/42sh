#include "lexer.h"
#include "parser.h"
#include "exp.h"

void	init_exp(t_exp *exp)
{
	ft_bzero(exp->buf, EXP_BSIZE);
	exp->i = 0;
	exp->quote = 0;
	exp->bs = 0;
	exp->param = NULL;
	exp->word = NULL;
	exp->res = NULL;
}

void	exp_flush_buf(t_exp *exp)
{
	char	*tmp;

	tmp = exp->res;
	if (!exp->res)
	{
		if (!(exp->res = ft_strdup(exp->buf)))
			exit(EXIT_FAILURE);
	}
	else if (!(exp->res = ft_strjoin(exp->res, exp->buf)))
		exit(EXIT_FAILURE);
	free(tmp);
	ft_bzero(exp->buf, exp->i);
	exp->i = 0;
}

void	exp_main(char **word, int assign)
{
	t_exp		exp;

	init_exp(&exp);
	if (assign)
		find_tilde_exp_assign(word, exp);
	else
		find_tilde_exp(word, exp);
//	ft_bzero(exp->buf, EXP_BSIZE);
//	parameter_exp(word, exp);
}

