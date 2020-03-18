
char	*resolve_parameter(char *str, int hash)
{
	int	i;
	char	*res;

	i = -1;
	if (!(ft_strlen(str) == 1 && (*str == '@' || *str == '*'
	|| *str == '#' || *str == '?' || *str == '-' || *str == '$'
	|| *str == '!' || ft_isdigit(*str)))
	|| (!ft_isname(str) && !(hash && *str == '#' && ft_isname(str + 1)))
		return (NULL);				// Bad Substitution
	if (!hash && !(res = find_var_value(cfg_shell()->env, str))
	&& !(res = ft_strnew(0)))
		exit(EXIT_FAILURE);
	else if (hash && !(res = find_var_value(cfg_shell()->env, str + 1))
	&& !(res = ft_strnew(0)))
		exit(EXIT_FAILURE);
	if (!res && !(res = ft_strdup(res)))
		exit(EXIT_FAILURE);
	return (res);
}

int	resolve_colon_param(char **str, t_exp *exp, char *param)
{
	char	*s;

	s = *str + 1;
	if (ft_strchri("-=?+", *s) < 0			//
	|| !(exp->param = resolve_parameter(param, 0)))	// Bad Substitution
		return (-1);				//
	if (exp->param[0])
	{
		if (*s == '+')
			return (substitute_word(exp, str, s));
		return (substitute_parameter(exp, str, s));
	}
	else if (*s == '-')
		return (substitute_word(exp, str, s));
	else if (*s == '=')
		return (assign_word(exp, str, s));
	else if (*s == '?')
		exit(EXIT_FAILURE);
	else
		return (substitute_null(exp, str, s));
}

int	resolve_brace_param(char **str, t_exp *exp, char *param)
{
	int	hash;
	char	*hashparam;

	*str++;
	hash = *param == '#' ? 1 : 0;
	if (!(exp->param = resolve_parameter(param, hash)))	// Bad
		return (-1);					// Substitution
	if (hash)
	{
		if (!(hashparam = ft_atoi(ft_strlen(exp->param))))
			exit(EXIT_FAILURE);
		exp_substitute(exp, hashparam);
		ft_strdel(&hashparam);
	}
	else
		exp_substitute(exp, exp->param);
	return (1);
}

int	resolve_pattern_param(char **str, t_exp *exp, char *param)
{

}
