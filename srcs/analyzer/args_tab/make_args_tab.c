#include "analyzer.h"
#include "struct.h"
#include "libft.h"
#include "sh.h"

int	args_tab_iter(t_list *lst, int (*f)(t_list *elem))
{
	while (lst != NULL)
	{
		if (f(lst) < 0)
			return (-1);
		lst = lst->next;
	}
	return (1);
}

int	mat_s_cmd(t_list *s_cmd_lst)
{
	t_simple_cmd	*cmd;

	cmd = (t_simple_cmd *)s_cmd_lst->data;
	if (word_expansions(cmd) < 0)
		return (-1);
	if (cmd->cmd_name)
	{
		if (!cmd->args)
		{
			if (!(cmd->av = ft_memalloc(sizeof(void*) * 2)))
				ft_ex(EXMALLOC);
			cmd->av[0] = cmd->cmd_name;
			cmd->av[1] = NULL;
			return (1);
		}
		cmd->av = (char **)args_to_tab(cmd->args, cmd->cmd_name);
	}
	else
		cmd->av = NULL;
	return (1);
}

int	mat_and_or(t_list *and_or_lst)
{
	t_and_or	*and_or;

	and_or = (t_and_or *)and_or_lst->data;
	return (args_tab_iter(and_or->s_cmd, &mat_s_cmd));
}

int	a_make_args_tab(t_cmd_table *table)
{
	return (args_tab_iter(table->and_or, &mat_and_or));
}
