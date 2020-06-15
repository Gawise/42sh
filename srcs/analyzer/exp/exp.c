#include "sh.h"
#include "analyzer.h"
#include "libft.h"
#include "ft_printf.h"
#include "var.h"

static int		exp_err(char *word)
{
	ft_dprintf(2, "%s: Bad Substitution\n", word);
	ft_setvar(&cfg_shell()->sp, "?", "1");
	return (-1);
}

static int		arg_exp(t_simple_cmd *cmd)
{
	t_list	*lst;
	int		ret;

	lst = cmd->args;
	while (lst)
	{
		if ((ret = exp_main((char **)&lst->data, 0)) < 0)
			return (ret == -1 ? exp_err((char *)lst->data) : -1);
		lst->data = (void *)a_quote_removal((char **)&lst->data);
		lst = lst->next;
	}
	return (1);
}

static int		assign_exp(t_simple_cmd *cmd)
{
	t_list			*lst;
	int				ret;
	t_assignment	*assign;

	lst = cmd->assign;
	while (lst)
	{
		assign = lst->data;
		if ((ret = exp_main((char **)&assign->val, 1)) < 0)
			return (ret == -1 ? exp_err((char *)assign->val) : -1);
		assign->val = a_quote_removal((char **)&assign->val);
		lst = lst->next;
	}
	return (1);
}

int				word_expansions(t_simple_cmd *cmd)
{
	int				ret;
	int				debug;

	if ((debug = cfg_shell()->debug))
		ft_dprintf(debug, "\n----------- expansions -----------\n\n");
	if (cmd->cmd_name && (ret = exp_main(&cmd->cmd_name, 0)) < 0)
		return (ret == -1 ? exp_err(cmd->cmd_name) : -1);
	if (cmd->cmd_name)
		cmd->cmd_name = (void *)a_quote_removal((char **)&cmd->cmd_name);
	if (arg_exp(cmd) < 0 || assign_exp(cmd) < 0)
		return (-1);
	return (1);
}

int				exp_main(char **word, int assign)
{
	t_exp		exp;
	int			ret;
	t_cfg		*cfg;

	cfg = cfg_shell();
	if (cfg->debug)
		ft_dprintf(cfg->debug, "Exp main start,\nstr= [%s]\n\n", *word);
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
		ft_dprintf(cfg->debug, "Exp main end,\nstr= [%s]\n\n", *word);
	return (1);
}
