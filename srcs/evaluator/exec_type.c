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

int		any_slash(t_list *var, t_process *p)
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
			return (127);
		return (0);
}

int		process_type(t_list *var, t_process *p)
{
	if (!ft_strchr(p->cmd, '/'))
		return (any_slash(var, p));
	p->setup += LOCATED;
	/* 127 NOT FOUND
	 * 126 NOT EXEC
	 * */
	return (0);
}
