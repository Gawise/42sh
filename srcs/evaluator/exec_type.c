#include "libft.h"
#include "parser.h"
#include "exec.h"


/*
 *Command Search and Execution
 *https://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html#tag_18_09_01_01
 */

int		builtin_search(void)
{
	return (0);
}

int		find_type(t_list *var, t_process *p)
{
	if (builtin_search())
		p->setup += BUILTIN;
	/*
	 *-> if utility {}
	 *-> if function {}
	 */
	else if ((p->path = ft_which(ft_getenv(var, "PATH"), p->cmd)))
		p->setup += EXEC;
	else
		return (p->setup += UNFOUND);
	return (0);
}


int		any_slash(t_list *var, t_process *p)
{
	if (find_type(var, p))
		p->retour = 127;
}

int		with_slash(t_process *p)
{
	p->setup += LOCATED;
	//verifier si il existe
}


int		process_type(t_list *var, t_process *p)
{
	if (!ft_strchr(p->cmd, '/'))
		any_slash(var, p);
	/* 127 NOT FOUND
	 * 126 NOT EXEC
	 * */
	return (0);
}
