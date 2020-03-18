#include "lexer.h"
#include "parser.h"
#include "exp.h"

int	get_tilde_exp(t_exp *exp)
{
	struct passwd	*pwd;

	if (!(pwd = getpwnam(exp->buf)))
		return (0);
	if (exp->res && !(exp->res = ft_sjoin1(&exp->res, pwd->pw_dir)))
		exit(EXIT_FAILURE);
	else if (!exp->res && !(exp->res = ft_strdup(pwd->pw_dir)))
		exit(EXIT_FAILURE);
	return (1);
}

int	get_home_val(t_exp *exp)
{
	char	*tilde;

	if (!tilde = get_tilde()) // need the HOME variable
		return (0);
	if (exp->res && !(exp->res = ft_sjoin1(&exp->res, tilde)))
		exit(EXIT_FAILURE);
	else if (!exp->res && !(exp->res = ft_strdup(tilde)))
		exit(EXIT_FAILURE);
	return 1;
}

int	parse_tilde_exp(char *str, t_exp *exp, int assign)
{
	int	i;

	i = 1;
	while (str[i])
	{
		if (ft_strchr("\'\"\\", str[i]))
			return (0);
		if (str[i] == '/' || (assign && str[i] == ':'))
			break ;
		exp->buf[exp->i] = str[i];
		i++;
		exp->i++;
	}
	if ((i == 1 && !get_home_val(exp)) || (i > 1 && !get_tilde_exp(exp)))
		return (0);
	return (i);
}

void	tilde_assign_dispatch(char **str, t_exp *exp, int *state)
{
	if (*state == 0 && **str == '~')
	{
		exp_flush_buf(exp)
		*str += parse_tilde_exp(*str, exp, 1);
		ft_bzero(exp->buf, EXP_BSIZE);
		exp->i = 0;
	}
	else if (ft_strchr("\'\"\\", **str))
	{
		if (**str == '\'' && exp->quote < 2)
			exp->quote = exp->quote == 1 ? 0 : 1;
		else if (**str == '\"' && exp->quote != 1)
			exp->quote = exp->quote == 2 ? 0 : 2;
		else if (**str == '\\' && exp->quote != 1)
			exp->bs = 2;
		state = 1;
	}
	else if (**str == ':')
		*state = -1;
}

void		find_tilde_exp_assign(t_list *word, t_exp exp)
{
	char		*str;
	int		state;

	if (!word || !word->data)
		return ;
	str = (char*)word->data;
	state = 0;
	while (*str)
	{
		if (!exp->bs && ft_strchr("~\'\"\\:", *str))
			tilde_assign_dispatch(str, &exp, &state);
		if (exp.i >= EXP_BSIZE - 1)
			exp_flush_buf(&exp);
		exp.buf[exp.i] = *str;
		if (exp.bs)
			exp.bs--;
		if (state < 1)
			state++;
		str++;
		exp.i++;
	}
	exp_flush_buf(&exp);
	free(word->data);
	word->data = exp->res;
}

void		find_tilde_exp(t_list *word, t_exp exp)
{
	char		*str;
	int		i;

	if (!word || !word->data)
		return ;
	str = (char*)word->data;
	if (*str != '~' || !(i = parse_tilde_exp(str, &exp, 0)))
		return ;
	while (str[i])
	{
		if (exp.i >= EXP_BSIZE - 1)
			exp_flush_buf(&exp);
		exp.buf[exp.i] = *str;
		str++;
		exp.i++;
	}
	exp_flush_buf(&exp);
	free(word->data);
	word->data = exp->res;
}
