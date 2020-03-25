#include "lexer.h"
#include "parser.h"
#include "exp.h"

int	simple_param_exp(t_exp *exp, char **str)
{
	char	*param;

	*str++;
	if (**str == '@' || **str == '*' || **str == '#' || **str == '?'
	|| **str == '-' || **str == '$' || **str == '!' || ft_isdigit(**str))
	{
		if (!(param = ft_strndup(*str, 1)))
			ft_ex("Cannot allocate memory\n");
	}
	else
	{
		while (**str && (ft_isalnum((int)**str) || **str != '_'))
			exp_add_to_buf(exp, str, &param);
		if (!exp->i)
		{
			*str--;
			return (0);
		}
		exp_flush_buf(exp, &param);
	}
	if (!(exp->param = resolve_parameter(param, 0)))
		return (-1);
	exp_substitute(exp, exp->param);
	return (1);
}

int	param_dispatch(t_exp *exp, char **str)
{
	if (**str == '$' && exp->quote != 1 && *(*str + 1) == '{')
	{
		exp_flush_buf(exp, &exp->res);
		return (rec_param_exp(exp, str));
	}
	else if (**str == '$' && exp->quote != 1)
	{
		exp_flush_buf(exp, &exp->res);
		return (simple_param_exp(exp, str));
	}
	else if (ft_strchr("\'\"\\", **str))
	{
		if (**str == '\'' && exp->quote < 2)
			exp->quote = exp->quote == 1 ? 0 : 1;
		else if (**str == '\"' && exp->quote != 1)
			exp->quote = exp->quote == 2 ? 0 : 2;
		else if (**str == '\\' && exp->quote != 1)
			exp->bs = 2;
	}
	return (1);
}

int	parse_param_exp(char **word, t_exp exp)
{
	char		*str;
	int		ret;

	if (!word || !*word)
		return (0);
	str = *word;
	ret = 0;
	while (*str)
	{
		if (!exp.bs && ft_strchr("\'\"\\$", *str)
		&& (ret = param_dispatch(&exp, &str) < 0))
			break ;
		exp_add_to_buf(&exp, &str, &exp.res);
		if (exp.bs)
			exp.bs--;
	}
	if (ret >= 0)
	{
		exp_flush_buf(&exp, &exp.res);
		free(*word);
		if (!(*word = ft_strdup(exp.res)))
			ft_ex("Cannot allocate memory\n");
	}
	free_exp_content(&exp);
	return (ret);
}
