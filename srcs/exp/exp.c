#include "sh.h"
#include "exp.h"
#include "struct.h"
#include "libft.h"
#include "ft_printf.h"
#include "var.h"

int	word_expansions(t_simple_cmd *cmd)
{
	t_list		*lst;
	t_assignment	*assign;

	if (cmd->cmd_name)
		exp_main(&cmd->cmd_name, 0);
	lst = cmd->args;
	while (lst)
	{
		exp_main((char **)&lst->data, 0);
		lst = lst->next;
	}
	lst = cmd->assign;
	while (lst)
	{
		assign = lst->data;
		exp_main((char **)&assign->val, 1);
		lst = lst->next;
	}
	return (1);
}

#include <stdio.h>

int	exp_main(char **word, int assign)
{
	t_exp		exp;

	printf("Entree exp main\nstr= [%s]\n\n", *word);
	init_exp(&exp);
	if (assign)
	{
		exp.assign = 1;
		find_tilde_exp_assign(word, exp);
	}
	else
	{
		exp.assign = 0;
		find_tilde_exp(word, exp);
	}
	ft_bzero(exp.buf, EXP_BSIZE);
	if (parse_param_exp(word, exp) < 0)
	{
		ft_dprintf(2, "%s: Bad Substitution\n", *word);
		ft_setvar(&cfg_shell()->env, "?", "1");
		return (-1);
	}
	printf("Sortie exp main\nstr= [%s]\n\n", *word);
	return (1);
}
