#include "libft.h"
#include "parser.h"
#include "exec.h"


/*
 *Command Search and Execution
 *https://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html#tag_18_09_01_01
 */

static int			builtin_search(void)
{
	return (0);
}

static uint16_t		find_type(t_list *var, t_process *p)
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


static void		any_slash(t_list *var, t_process *p)
{
	if (find_type(var, p))
		return ;
	p->setup |= path_errors(p->path);
}

static void		with_slash(t_process *p)
{
	char		*buf;
	char		*tmp;

	p->setup |= SLASH;
	p->path = ft_strdup(p->cmd);
	if (*p->path == '/')
		p->setup |= path_errors(p->path);
	else
	{
		if (!(buf = getcwd(0, 0)))
			ex("getcwd");
		tmp = ft_strjoin(3, buf, "/", p->path);
		p->setup |= path_errors(tmp);
		ft_strdel(&buf);
		ft_strdel(&tmp);
	}
}

void	process_type(t_list *var, t_process *p)
{
	if (ft_strchr(p->cmd, '/'))
		with_slash(p);
	else (any_slash(var, p));
}
