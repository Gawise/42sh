#include "exp.h"
#include "libft.h"

int	word_dispatch(t_exp *exp, char **str, int *quote)
{
	if (**str == '$' && *quote != 1 && *(*str + 1) == '{')
	{
		exp_add_to_buf(exp, str, &exp->word);
		return (rec_word_parse(exp, str) == -1 ? -1 : 0);
	}
	else if (**str == '}' && !*quote)
	{
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

int	skip_word_dispatch(t_exp *exp, char **str, int *quote)
{
	if (**str == '$' && *quote != 1 && *(*str + 1) == '{')
	{
		(*str)++;
		return (skip_word(exp, str) == -1 ? -1 : 0);
	}
	else if (**str == '}' && !*quote)
	{
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

#include <stdio.h>

int	skip_word(t_exp *exp, char **str)
{
	int	quote;
	int	ret;

	printf("\nentree dans skip str = [%s]\n", *str);
	quote = 0;
	ret = 0;
	while (**str)
	{
		if (!exp->bs && ft_strchr("\'\"\\$}", **str)
		&& (ret = skip_word_dispatch(exp, str, &quote)))
			break ;
		if (exp->bs)
			exp->bs--;
		(*str)++;
	}
	if (!ret)
		return (-1);
	printf("sortie de skip, str = [%s]\n", *str);
	return (ret);
}

int	rec_word_parse(t_exp *exp, char **str)
{
	int	quote;
	int	ret;

	printf("\nentree rec word str = [%s]\n", *str);
	quote = 0;
	ret = 0;
	while (**str)
	{
		if (!exp->bs && ft_strchr("\'\"\\$}", **str)
		&& (ret = word_dispatch(exp, str, &quote)))
			break ;
		exp_add_to_buf(exp, str, &exp->word);
		if (exp->bs)
			exp->bs--;
	}
	printf("rec word ret = %d, str = [%s]\n", ret, *str);
	if (!ret)
		return (-1);
	return (ret);
}
