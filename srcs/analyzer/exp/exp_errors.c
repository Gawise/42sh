#include "analyzer.h"
#include "sh.h"
#include "ft_printf.h"
#include "var.h"

int		exp_err(char *word)
{
	ft_dprintf(2, "%s: Bad Substitution\n", word);
	ft_setvar(&cfg_shell()->sp, "?", "1");
	return (-1);
}

int		print_error_parameter(t_exp *exp, char **str, char *param)
{
	int	ret;

	if ((ret = substitute_word(exp, str)) < 0)
		return (ret);
	if (!exp->word || !*exp->word)
	{
		free(exp->word);
		if (!(exp->word = ft_strdup("parameter null or not set")))
			ft_ex(EXMALLOC);
	}
	ft_dprintf(2, "21sh: %s: %s\n", param, exp->word);
	return (-2);
}
