#include "lexer.h"
#include "parser.h"
#include "exp.h"

void	init_exp(t_exp *exp)
{
	ft_bzero(exp->buf, EXP_BSIZE);
	exp->i = 0;
	exp->res = NULL;
	exp->quote = 0;
	exp->bs = 0;
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

void	exp_dispatch(char **str, t_exp *exp)
{

	if (**str == '\'' && exp->quote < 2)
	{
		exp->quote = exp->quote == 1 ? 0 : 1;
		*str++;
	}
	else if (**str == '\"' && exp->quote != 1)
	{
		exp->quote = exp->quote == 2 ? 0 : 2;
		*str++;
	}
	else if (**str == '$' && exp->quote != 1)
		parameter_exp(str, exp);


//	else if (**str == '{')
//		return ;
//	else if (**str == '(')
//		return ;
}

void	exp_main(t_list *word, int assign)
{
	t_exp		exp;

	init_exp(&exp);
	if (assign)
		find_tilde_exp_assign(word, exp);
	else
		find_tilde_exp(word, exp);
	parameter_exp(word, exp);
	ft_bzero(exp->buf, EXP_BSIZE);
	
	ft_bzero(exp->buf, EXP_BSIZE);
}

