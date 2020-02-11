#include "parser.h"
#include "exec.h"
#include "libft.h"

#include <stdio.h>
void	clean_all(t_cmd_ctable *cmdt, t_and_or *andor, t_simple_cmd *s1, t_simple_cmd *s2)
{
	ft_bzero(cmdt, sizeof(t_cmd_ctable));
	ft_bzero(andor, sizeof(t_and_or));
	ft_bzero(s1, sizeof(t_simple_cmd));
	ft_bzero(s2, sizeof(t_simple_cmd));
}

void	print(t_list *lst)
{
	t_simple_cmd *s;
	while (lst)
	{
		s = lst->data;
		lst = lst->next;
	}
}

int		find_pipe(char **av)
{
	int i = 0;

	while (av[i] && *av[i] != '|')
		i++;
	if (!av[i])
		return -1;
	av[i] = NULL;
	return i;
}

void	set_all(t_cmd_ctable *cmdt, t_and_or *andor, t_simple_cmd *s1, t_simple_cmd *s2, char **av)
{

	int pipe = find_pipe(av);
	s1->cmd_name = *av;
	s1->arguments = av;
	andor->s_cmd = ft_lstnew(s1, sizeof(t_simple_cmd));

	if (pipe != -1)
	{
		s2->cmd_name = *(av + pipe + 1);
		s2->arguments = av + pipe + 1;
		ft_lst_push_back(&andor->s_cmd, s2, sizeof(t_simple_cmd));
	}

	print(andor->s_cmd);

	cmdt->and_or = ft_lstnew(andor, sizeof(t_and_or));

}

int		main(int ac, char **av, char **env)
{
	if (ac < 2)
		return (0);

	write(1, "main\n", 5);
	t_cmd_ctable		cmdt;
	t_and_or		andor;
	t_simple_cmd	s1;
	t_simple_cmd	s2;
	t_list			*lst;
	
	clean_all(&cmdt, &andor, &s1, &s2);
	set_all(&cmdt, &andor, &s1, &s2, av+1);
	lst = ft_lstnew(&cmdt, sizeof(cmdt));

	/*****************************************/





	lvl_cmd_ctable(&cfg, lst);

	exit(0);
}
