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
