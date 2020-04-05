#include "exp.h"
#include "sh.h"
#include "libft.h"
#include "var.h"

char	*resolve_parameter(char *str, int hash)
{
	int	i;
	char	*res;

	i = -1;
	if (!(ft_strlen(str) == 1 && (*str == '@' || *str == '*'
	|| *str == '#' || *str == '?' || *str == '-' || *str == '$'
	|| *str == '!' || ft_isdigit(*str)))
	&& (!ft_isname(str) && !(hash && *str == '#' && ft_isname(str + 1))))
		return (NULL);
	if (!hash && !(res = find_var_value(cfg_shell()->env, str))
	&& !(res = ft_strnew(0)))
		ft_ex("Cannot allocate memory\n");
	else if (hash && !(res = find_var_value(cfg_shell()->env, str + 1))
	&& !(res = ft_strnew(0)))
		ft_ex("Cannot allocate memory\n");
	if (res && !(res = ft_strdup(res)))
		ft_ex("Cannot allocate memory\n");
	return (res);
}

#include <stdio.h>
int	resolve_colon_param(char **str, t_exp *exp, char *param)
{
	(*str)++;
	printf("\nentree colon param str = [%s]\n", *str);
	if (!ft_strchr("-=?+", **str)
	|| !(exp->param = resolve_parameter(param, 0)))
		return (-1);
	printf("encore bon colon\n");
	if (exp->param[0])
	{
		if (**str == '+')
			return (substitute_word(exp, str));
		return (substitute_parameter(exp, str));
	}
	else if (**str == '-')
		return (substitute_word(exp, str));
	else if (**str == '=')
		return (assign_word(exp, str, param));
	else if (**str == '?')
		ft_ex("Cannot allocate memory\n");
	else
		return (substitute_null(exp, str));
	return (0);
}

int	resolve_brace_param(char **str, t_exp *exp, char *param)
{
	int	hash;
	char	*hashparam;

	(*str)++;
	hash = *param == '#' ? 1 : 0;
	if (!(exp->param = resolve_parameter(param, hash)))
		return (-1);
	if (hash)
	{
		if (!(hashparam = ft_itoa(ft_strlen(exp->param))))
			ft_ex("Cannot allocate memory\n");
		exp_substitute(exp, hashparam);
		ft_strdel(&hashparam);
	}
	else
		exp_substitute(exp, exp->param);
	return (1);
}

int	resolve_pattern_param(char **str, t_exp *exp, char *param)
{
	int	type;

	type = **str == '#' ? 0 : 1;
	if (ft_strchr("#%", *(*str + 1)))
		(*str)++;
	str++;
	if (!(exp->param = resolve_parameter(param, 0))
	|| rec_word_parse(exp, str) < 0
	|| exp_main(&exp->word, exp->assign) < 0)
		return (-1);
	substitute_pattern(exp, type);
	return (1);
}
