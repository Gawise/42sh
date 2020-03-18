#include "lexer.h"
#include "parser.h"
#include "exp.h"

/*
void		parameter_exp(char **word, t_exp *exp)
{
	char	*str;

	str = *word + 1;
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
*/

void	exp_flush_buf_param(t_exp *exp, char **param)
{
	char	*tmp;

	tmp = *param;
	if (!*param)
	{
		if (!(*param = ft_strdup(exp->buf)))
			exit(EXIT_FAILURE);
	}
	else if (!(*param = ft_strjoin(*param, exp->buf)))
		exit(EXIT_FAILURE);
	free(tmp);
	ft_bzero(exp->buf, exp->i);
	exp->i = 0;
}

int	rec_param_dispatch(char **str, t_exp *exp, char **param)
{
	if (**str == '#' && !exp->i)
		return (0);
	else if (**str == ':')
	{
		exp_flush_buf_param(exp, param);
		return (resolve_colon_param(str, exp, *param));
	}
	else if (**str == '}')
	{
		exp_flush_buf_param(exp, param);
		return (resolve_brace_param(str, exp, *param))
	}
	else if (ft_strchr("#%", **str))
	{
		exp_flush_buf_param(exp, param);
		return (resolve_pattern_param(str, exp, *param));
	}
	return (0);
}

int	rec_param_exp(char **str, t_exp *exp)
{
	char	*s;
	char	*param;
	int	ret;

	s = *str + 2;
	param = NULL;
	while (*s)
	{
		if (!exp.bs && ft_strchr(":}%#", *s)
		&& (ret = rec_param_dispatch(&s, exp, &param)))
			break ;
		if (exp.i >= EXP_BSIZE - 1)
			exp_flush_buf_param(exp, param);
		exp.buf[exp.i] = *s;
		if (exp->bs)
			exp->bs--;
		s++;
		exp.i++;
	}
	*str = s;
	ft_strdel(&param);
	return (ret)
}

int	param_dispatch(char **str, t_exp *exp)
{
	if (**str == '$' && exp->quote != 1 && *(*str + 1) == '{')
	{
		exp_flush_buf(&exp);
		return (rec_param_exp(str, exp))
	}
	else if (**str == '$' && exp->quote != 1)
	{
		exp_flush_buf(&exp);
		return (simple_param_exp(str, exp, exp->quote))
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

int	parse_param_exp(t_list *word, t_exp exp)
{
	char		*str;

	if (!word || !word->data)
		return ;
	str = (char*)word->data;
	while (*str)
	{
		if (!exp.bs && ft_strchr("\'\"\\$", *str))
			param_dispatch(&str, &exp);
		if (exp.i >= EXP_BSIZE - 1)
			exp_flush_buf(&exp);
		exp.buf[exp.i] = *str;
		if (exp->bs)
			exp->bs--;
		str++;
		exp.i++;
	}
	free_exp_content()
}
