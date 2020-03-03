#include "libft.h"
#include "parser.h"
#include "exec.h"
#include "var.h"
#include "sh.h"


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
	else if ((p->path = ft_which(find_var_value(var, "PATH"), p->cmd)))
		p->setup |= EXEC;
	else
		return (p->setup |= E_UNFOUND);
	return (SUCCESS);
}


static void		any_slash(t_list *var, t_process *p)
{
	if (find_type(var, p))
		return ;
	p->setup |= path_errors(p->path, 1);
}


char		*create_abs_path(char *s)
{
	char		*buf;
	char		*tmp;

	if (!(buf = getcwd(0, 0)))
		ex("getcwd");
	tmp = ft_strjoin(3, buf, "/", s);
	ft_strdel(&buf);
	return (tmp);
}

static void		with_slash(t_process *p)
{
	char		*tmp;

	p->setup |= SLASH;
	p->path = ft_strdup(p->cmd);
	if (*p->path == '/')
		p->setup |= path_errors(p->path, 1);
	else
	{
		tmp = create_abs_path(p->path);
		p->setup |= path_errors(tmp, 1);
		ft_strdel(&tmp);
	}
}

void	process_type(t_list *var, t_process *p)
{
	if (ft_strchr(p->cmd, '/'))
		with_slash(p);
	else (any_slash(var, p));
}
