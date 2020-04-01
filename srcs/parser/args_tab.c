#include "libft.h"
#include "lexer.h"
#include "parser.h"
#include "exp.h"

void		**args_to_tab(t_list *lst, char *cmd)
{
	void	**tab;
	t_list	*index;
	int	i;

	index = lst;
	i = 1;
	while ((index = index->next))
		i++;
	if (!(tab = ft_memalloc(sizeof(void *) * (i + 2))))
		exit(EXIT_FAILURE);
	index = lst;
	i = 1;
	tab[0] = cmd;
	while (index)
	{
		tab[i] = index->data;
		index = index->next;
		i++;
	}
	tab[i] = NULL;
	return (tab);
}

void	p_mat_s_cmd(t_list *s_cmd_lst)
{
	t_simple_cmd	*cmd;

	cmd = (t_simple_cmd *)s_cmd_lst->data;
	word_expansions(cmd);
	if (cmd->cmd_name)
	{
		if (!cmd->args)
		{
			cmd->av = ft_memalloc(sizeof(void*) * 2);
			cmd->av[0] = cmd->cmd_name;
			cmd->av[1] = NULL;
			return ;
		}
		cmd->av = (char **)args_to_tab(cmd->args, cmd->cmd_name);
	}
	else
		cmd->av = NULL;
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
