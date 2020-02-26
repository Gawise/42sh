#include "libft.h"
#include "parser.h"
#include "exec.h"


/*
 *Command Search and Execution
 *https://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html#tag_18_09_01_01
 */

static int		builtin_search(void)
{
	return (0);
}

static uint16_t	find_type(t_list *var, t_process *p)
{
	if (builtin_search())
		p->setup |= BUILTIN;
	/*
	 *-> if utility {}
	 *-> if function {}
	 */
	else if ((p->path = ft_which(ft_getenv(var, "PATH"), p->cmd)))
		p->setup |= EXEC;
	else
		return (p->setup |= E_UNFOUND);
	return (SUCCESS);
}


static uint8_t	any_slash(t_list *var, t_process *p)
{
	 if (find_type(var, p) == SUCCESS)
		return (SUCCESS);
	return (1);
}

static void		with_slash(t_process *p)
{
	p->setup |= SLASH;
	p->path = ft_strdup(p->cmd);
}


uint8_t		process_type(t_list *var, t_process *p)
{
	uint16_t	error;

	if (ft_strchr(p->cmd, '/'))
		with_slash(p);
	else if (any_slash(var, p))
		return (FAILURE);
	if ((error = path_errors(p->path)))
		p->setup |= error;
	printf("p->setup = [%d]\n", p->setup);
	printf("path = [%s]\n", p->path);
	//rajouter PWD dans PATH ou seulement dans checkPATH ????



	/* 127 NOT FOUND
	 * 126 NOT EXEC
	 * */
	return (0);
}
