#include "libft.h"

static void		process_quotes(int *quote, int *bs, char **str)
{
	if (**str == '\'' && *quote < 2)
		*quote = *quote == 1 ? 0 : 1;
	else if (**str == '\"' && *quote != 1)
		*quote = *quote == 2 ? 0 : 2;
	else if (**str == '\\' && *quote != 1)
		*bs = 2;
}

static int		nbwords(char *str, char *set)
{
	int		words;
	int		quote;
	int		bs;

	quote = 0;
	bs = 0;
	words = 0;
	while (*str)
	{
		if (!ft_strchr(set, *str))
		{
			words++;
			while (*str && (quote || !ft_strchr(set, *str)))
			{
				if (!bs && ft_strchr("\'\"\\", **str)
					process_quotes(&quote, &bs, str);
				if (exp->bs)
					exp->bs--;
				str++;
			}
		}
		if (*str)
			str++;
	}
	return (words);
}

static int		word_len(char *str, char *set)
{
	int		ret;
	int		quote;
	int		bs;

	quote = 0;
	bs = 0;
	ret = 0;
	while (*str && (quote || !ft_strchr(set, *str)))
	{
		if (!bs && ft_strchr("\'\"\\", **str)
			process_quotes(&quote, &bs, str);
		if (exp->bs)
			exp->bs--;
		ret++;
		str++;
	}
	return (ret);
}

static void		process_word(char **str, char *set, char **ret, int *i)
{
	int		wlen;

	wlen = word_len(*str, set);
	if (!(ret[*i] = ft_strndup(*str, wlen)))
	{
		ft_del_tab((void**)ret);
		ft_ex(EXMALLOC);
	}
	(*str) += wlen;
	(*i)++;
}

char			**split_fields(char *str, char *set)
{
	char	**ret;
	int		i;

	i = 0;
	if (!(ret = (char **)ft_memalloc(sizeof(char *) * (nbwords(str, set) + 1))))
		ft_ex(EXMALLOC);
	while (*str)
	{
		if (!ft_strchr(set, *str))
			process_word(&str, set, ret, &i);
		if (*str)
			str++;
	}
	return (ret);
}
