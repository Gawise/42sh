#include "libft.h"
#include "ft_printf.h"
#include "lexer.h"
#include "parser.h"

int	get_nb_cmd(t_list *cmd)
{
	int nb = 1;

	if (!cmd->data)
		return (0);
	while ((cmd = cmd->next))
		nb++;
	return (nb);
}

int	get_nb_and_or(t_list *and_or)
{
	int nb = 1;

	if (!and_or->data)
		return (0);
	while ((and_or = and_or->next))
		nb++;
	return (nb);
}

int	get_nb_word_list(t_list *list)
{
	int nb = 1;

	if (!list->data)
		return (0);
	while ((list = list->next))
		nb++;
	return (nb);
}

void	print_assignment(t_list *assignment)
{
	t_assignment	*assign;

	assign = (t_assignment *)assignment->data;
	ft_printf("\t\t| %13s | %13s |\n", assign->var, assign->val);
}

char	*get_redir_op(t_token_type type)
{
	if (type == 10)
		return (ft_strdup("<"));
	if (type == 11)
		return (ft_strdup("<<"));
	if (type == 12)
		return (ft_strdup(">"));
	if (type == 13)
		return (ft_strdup(">>"));
	if (type == 14)
		return (ft_strdup("<&"));
	if (type == 15)
		return (ft_strdup(">&"));
	if (type == 16)
		return (ft_strdup("<<-"));
	return (NULL);
}

void	print_redir(t_list *redirection)
{
	t_redir		*redir;
	char		*operator;

	redir = (t_redir *)redirection->data;
	operator = get_redir_op(redir->type);
	if (redir->io_num >= 0)
		ft_printf("\t\t| %7d | %6s | %10s |\n", redir->io_num, operator, redir->file);
	else
		ft_printf("\t\t| %7c | %6s | %10s |\n", '-', operator, redir->file);
	free(operator);
}

void	print_s_cmd(t_list *cmd_list)
{
	t_list		*args;
	int		n_arg;
	int		nb_args;
	t_simple_cmd	*cmd;

	cmd = (t_simple_cmd*)cmd_list->data;
	args = cmd->args;
	if (args && (nb_args = get_nb_word_list(args)))
		n_arg = 1;
	if (cmd->cmd_name)
	{
		ft_printf("\t\t\033[1m-----------------\033[0m\n");
		ft_printf("\t\t| %13s |\n", "cmd_name");
		ft_printf("\t\t| %13s |\n", cmd->cmd_name);
		ft_printf("\t\t-----------------\n");
	}
	if (args)
	{
		ft_printf("\t\t| %13s |\n", "arguments");
		while (n_arg <= nb_args)
		{
			ft_printf("\t\t| %2d %10.10s |\n", n_arg, (char *)args->data);
			n_arg++;
			args = args->next;
		}
		ft_printf("\t\t-----------------\n");
	}
	if (cmd->assign)
	{
		if (cmd->cmd_name)
			ft_printf("\n");
		ft_printf("\t\t---------------------------------\n");
		ft_printf("\t\t| %13s | %13s |\n", "assign_var", "assign_val");
		ft_lstiter(cmd->assign, print_assignment);
		ft_printf("\t\t---------------------------------\n");
	}
	if (cmd->redir)
	{
		if (cmd->cmd_name || cmd->assign)
			ft_printf("\n");
		ft_printf("\t\t---------------------------------\n");
		ft_printf("\t\t| %7s | %6s | %10s |\n", "io_nbr", "type", "file");
		ft_lstiter(cmd->redir, print_redir);
		ft_printf("\t\t---------------------------------\n");
	}

//	ft_printf("| %10s | %10s | %10s |\n", "nb_assign", "nb_redir");
//	ft_printf("| %10d | %10d | %10d |\n", get_nb_assign(), get_nb_redir());
}

char	*get_and_or_type_str(t_and_or *and_or)
{
	if (and_or->type == AND_IF)
		return (ft_strdup("AND_IF"));
	else if (and_or->type == OR_IF)
		return (ft_strdup("OR_IF"));
	else
		return (NULL);
}

void	print_andor(t_list *and_or)
{
	t_and_or	*and_or_cont;
	t_list		*cmd_list;
	int		n_cmd = 1;

	and_or_cont = (t_and_or *)and_or->data;
	cmd_list = (t_list*)and_or_cont->s_cmd;
	ft_printf("\t----------------------------------------\n");
	ft_printf("\t| %10s | %10s | %10s |\n", "type", "cmd_nb", "background");
	ft_printf("\t| %10s | %10d | %10d |\n", get_and_or_type_str(and_or_cont), get_nb_cmd(cmd_list), and_or_cont->background);
	ft_printf("\t----------------------------------------\n\n");
	while (cmd_list)
	{
		ft_printf("\t\tcmd %d:\n", n_cmd);
		print_s_cmd(cmd_list);
		n_cmd++;
		cmd_list = cmd_list->next;
	}
	ft_printf("\n");
}

void	print_cmd_table(t_list *table)
{
	t_list		*and_or;
	t_cmd_table	*table_cont;
	//t_simple_cmd	*s_cmd;
	//t_redir		*redir;
	//t_assignment	*assign;
	int		n_andor = 1;

	table_cont = (t_cmd_table *)table->data;
	if (table_cont)
	{
		ft_printf("table=%p\n", table_cont);
		ft_printf("--------------\n");
		ft_printf("| %10s |\n", "and_or nb");
		ft_printf("| %10d |\n", get_nb_and_or(table_cont->and_or));
		ft_printf("--------------\n\n");
	}
	and_or = table_cont->and_or;
	if (and_or)
	{
		while (and_or)
		{
			ft_printf("\tand_or %d:\n", n_andor);
			print_andor(and_or);
			n_andor++;
			and_or = and_or->next;
		}
	}
}

void	print_parser(t_parser *parser)
{
	t_list	*table;
	int i = 1;

	table = parser->table;
	while (table)
	{
		ft_printf("table %d\n", i++);
		print_cmd_table(table);
		table = table->next;
	}
}
