#include "lexer.h"
#include "parser.h"
#include "exp.h"

t_exp	*init_exp()
{
	t_exp	*exp;

	if (!(exp = memalloc(sizeof(t_exp *))))
		exit(EXIT_FAILURE);
	ft_bzero(exp->buf, EXP_BSIZE);
	exp->i = 0;
	exp->res = NULL;
	exp->quote = 0;
	return (exp);
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
	ft_bzero(exp->buf, EXP_BSIZE);
	exp->i = 0;
}

void	exp_dispatch(char **str, char buf[EXP_BSIZE], int *i, char **res)
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

void	resolve_expansions(t_list *word)
{
	t_exp		*exp;
	char		*str;

	if (!word || !word->data)
		return ;
	exp = init_exp(word);
	str = (char*)word->data;
	while (*str)
	{
		if (ft_strchr("\'\"${(", *str) && *(str - 1) != '\\')
			exp_dispatch(&str, exp);
		if (exp->i >= EXP_BSIZE - 1)
			exp_flush_buf(exp);
		exp->buf[exp->i] = *str;
		str++;
		i++;
	}

	free_exp();
}
