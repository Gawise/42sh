#include "sh.h"
#include "analyzer.h"
#include "libft.h"
#include "ft_printf.h"
#include "var.h"

int	exp_err(char *word)
{
	ft_dprintf(2, "%s: Bad Substitution\n", word);
	ft_setvar(&cfg_shell()->env, "?", "1");
	return (-1);
}

int	word_expansions(t_simple_cmd *cmd)
{
	t_list		*lst;
	t_assignment	*assign;
	int		ret;

	if (cfg_shell()->debug)
		ft_printf("\n----------- expansions -----------\n\n");
	if (cmd->cmd_name && (ret = exp_main(&cmd->cmd_name, 0)) < 0)
		return (ret == -1 ? exp_err(cmd->cmd_name) : -1);
	lst = cmd->args;
	while (lst)
	{
		if ((ret = exp_main((char **)&lst->data, 0)) < 0)
			return (ret == -1 ? exp_err((char *)lst->data) : -1);
		lst = lst->next;
	}
	lst = cmd->assign;
	while (lst)
	{
		assign = lst->data;
		if ((ret = exp_main((char **)&assign->val, 1)) < 0)
			return (ret == -1 ? exp_err((char *)assign->val) : -1);
		lst = lst->next;
	}
	return (1);
}

int	exp_main(char **word, int assign)
{
	t_exp		exp;
	int		ret;
	t_cfg		*cfg;

	cfg = cfg_shell();
	if (cfg->debug)
		ft_printf("Exp main start,\nstr= [%s]\n\n", *word);
	init_exp(&exp);
	exp.assign = assign;
	if (assign)
		find_tilde_exp_assign(word, exp);
	else
		find_tilde_exp(word, exp);
	ft_bzero(exp.buf, EXP_BSIZE);
	if ((ret = parse_param_exp(word, exp)) < 0)
		return (ret);
	if (cfg->debug)
		ft_printf("Exp main end,\nstr= [%s]\n\n", *word);
	return (1);
}
