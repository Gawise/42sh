#include "lexer.h"
#include "parser.h"
#include "exp.h"

void		parameter_exp(char **word, t_exp *exp)
{
	char	*str;
	int	brace;

	str = *word + 1;
	brace = 0;
	if (*str == '{')
	{
		brace = 1;
		str++;
	}
	if (ft_isdigit(*str))
		return ;



	while (*str)
	{
		if (ft_strchr("\'\"}", *str) && *(str - 1) != '\\')
			exp_dispatch(&str, exp);
		if (exp->i >= EXP_BSIZE - 1)
			exp_flush_buf(exp);
		exp->buf[exp->i] = *str;
		str++;
		i++;
	}
	
	
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

}

void	parse_param_exp(t_list *word, t_exp exp)
{
	char		*str;
	int		nb;

	if (!word || !word->data)
		return ;
	str = (char*)word->data;
	nb = 0;
	while (*str)
	{
		if (*str == '$' ft_strchr("\'\"$", *str) && nb && *(str - 1) != '\\')
			exp_dispatch(&str, &exp);
		if (exp.i >= EXP_BSIZE - 1)
			exp_flush_buf(&exp);
		exp.buf[exp.i] = *str;
		str++;
		exp.i++;
		nb++;
	}
	free_exp_content()
}
