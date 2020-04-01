#include "exp.h"
#include "libft.h"

int	word_dispatch(t_exp *exp, char **str, int *quote, int skip)
{
	if (**str == '$' && *quote != 1 && *(*str + 1) == '{')
	{
		if (skip)
			return (skip_word(exp, str));
		exp_flush_buf(exp, &exp->word);
		return (rec_word_parse(exp, str) == -1 ? -1 : 0);
	}
	else if (**str == '}' && !*quote)
	{
		if (!skip)
			exp_flush_buf(exp, &exp->word);
		return (1);
	}
	else if (ft_strchr("\'\"\\", **str))
	{
		if (**str == '\'' && *quote < 2)
			*quote = *quote == 1 ? 0 : 1;
		else if (**str == '\"' && *quote != 1)
			*quote = *quote == 2 ? 0 : 2;
		else if (**str == '\\' && *quote != 1)
			exp->bs = 2;
	}
	return (0);
}

int	skip_word(t_exp *exp, char **str)
{
	int	quote;
	int	ret;

	quote = 0;
	ret = 0;
	while (**str)
	{
		if (!exp->bs && ft_strchr("\'\"\\$}", **str)
		&& (ret = word_dispatch(exp, str, &quote, 1)))
			break ;
		if (exp->bs)
			exp->bs--;
	}
	if (!ret)
		return (-1);
	return (ret);
}

int	rec_word_parse(t_exp *exp, char **str)
{
	int	quote;
	int	ret;

	quote = 0;
	ret = 0;
	while (**str)
	{
		if (!exp->bs && ft_strchr("\'\"\\$}", **str)
		&& (ret = word_dispatch(exp, str, &quote, 0)))
			break ;
		exp_add_to_buf(exp, str, &exp->word);
		if (exp->bs)
			exp->bs--;
	}
	if (!ret)
		return (-1);
	return (ret);
}
