#include "libft.h"
#include "lexer.h"
#include "parser.h"

void	p_mat_s_cmd(t_list *s_cmd_lst)
{
	t_simple_cmd	*cmd;

	cmd = (t_simple_cmd *)s_cmd_lst->data;
	cmd->av = (char**)lst_to_tab(cmd->args);
}

void	p_mat_and_or(t_list *and_or_lst)
{
	t_and_or	*and_or;

	and_or = (t_and_or *)and_or_lst->data;
	ft_lstiter(and_or->s_cmd, &p_mat_s_cmd);
}

void	p_mat_table(t_list *table_lst)
{
	t_cmd_table	*table;

	table = (t_cmd_table *)table_lst->data;
	ft_lstiter(table->and_or, &p_mat_and_or);
}

void	p_make_args_tab(t_parser *parser)
{
	ft_lstiter(parser->table, &p_mat_table);
}
