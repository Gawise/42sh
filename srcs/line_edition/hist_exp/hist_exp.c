#include "line_edition.h"
#include "ft_printf.h"

int		exp_last_cmd(t_exp *exp)
{
	char	*res;

	if (!(exp->word = ft_itoa(histsize - 1)))//nombre total d'entrees dans l'historique
		ft_ex(EXMALLOC);
	if (!(res = ft_hash_lookup(cfg_shell()->history, exp->word)))
		return (0);
	exp_substitute(exp, res);
	return (1);
}

int		exp_digit(char **src, t_exp *exp)
{
	int		val;
	char	*res;
	
	val = ft_atoi(*src);
	if (val < 1 || val >= histsize)//nombre total d'entrees dans l'historique
		return (0);
	if (!(exp->word = ft_itoa(val)))
		ft_ex(EXMALLOC);
	if (!(res = ft_hash_lookup(cfg_shell()->history, exp->word)))
		return (0);
	exp_substitute(exp, exp->word);
	return (1);
}

int		exp_minus(char **src, t_exp *exp)
{
	int		histsize;
	int		val;
	char	*res;

	val = ft_atoi(*src);
	histsize = 100000000000; //nombre total d'entrees dans l'historique
	if (val || val < -(histsize))
		return (0);
	val += histsize;
	if (!exp->word = ft_itoa(val))
		ft_ex(EXMALLOC);
	if (!(res = ft_hash_lookup(cfg_shell()->history, exp->word)))
		return (0);
	exp_substitute(exp, res);
	return (1);
}

int		exp_word(char **src, t_exp *exp)
{
	int		index;
	char	*res;

	while (!ft_strchr(" \t\n", *str) && !ft_strchr(HIST_DELIM, *str))
		exp_add_to_buf(exp, src, exp->word);
	exp_flush_buf(exp, exp->word);
	index = 10000000000; //nombre total d'entrees dans l'historique
	while (index)
	{
		if (!(res = ft_hash_lookup(cfg_shell()->history, ft_itoa(index))))
			return (0);
		if (ft_strnequ(val, exp->word, ft_strlen(exp->word)))
		{
			exp_substitute(exp, res);
			return (1);
		}
		index--;
	}
	return (0);
}

int		parse_hist_exp(char **src, t_exp *exp)
{
	char	*str;
	int		ret;

	str = *src;
	if (*str == '!')
		ret = exp_last_cmd(exp);
	else if (ft_isdigit(*str))
		ret = exp_digit(src, exp);
	else if (*str == '-')
		ret = exp_minus(src, exp);
	else
		ret = exp_word(src, exp);
	if (ret)
		ft_strdel(&exp->word);
	return (ret);
}

int		handle_exp_flag(char **src, t_exp *exp)
{
	char	*str;

	str = *src;
	if (ft_strchr(" \t\n", *(str + 1)))
	{
		exp_add_to_buff(exp, src, exp->res);
		return (1);
	}
	if (!ft_strchr(HIST_DELIM, *(str + 1)))
	{
		(*src)++;
		exp_flush_buf(exp, exp->res);
		return (parse_hist_exp(src, exp));
	}
	return (0);
}

static int	hist_exp_error(t_exp *exp)
{
	ft_dprintf(2, "%s: !%s: event not found\n", PROJECT, exp->word);
	free_exp_content(exp);
	return (0);
}

int		expand_history(char **line)
{
	char	*str;
	t_exp	exp;

	str = *line;
	init_exp(&exp);
	while (*str)
	{
		if (*str == '!' && !handle_exp_flag(&str, &exp))
			return (hist_exp_error(&exp));
		exp_add_to_buf(&exp, &str, &exp.res);
	}
	ft_strdel(line);
	*line = exp.res;
	ft_printf("%s", exp.res);
	free_exp_content(&exp);
}
