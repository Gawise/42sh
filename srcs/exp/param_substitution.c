
void	exp_substitute(t_exp *exp, char *str)
{
	char	*res;

	if ((!exp->res && !(res = ft_strdup(str)))
	|| (exp->res && !(res = ft_strjoin(exp->res, str))))
		exit(EXIT_FAILURE);
	free(exp->res);
	exp->res = res;
}

int	substitute_parameter()
{

}

int	substitute_word()
{

}

int	assign_word()
{

}

int	substitute_null()
{

}
