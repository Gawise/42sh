#include "libft.h"
#include "parser.h"
#include "exec.h"
#include "var.h"
#include "sh.h"

uint32_t			builtin_search(t_process *p)
{
	if (!p->cmd)
		return (0);
	if (!ft_strcmp(p->cmd, "echo"))
		return (p->setup |= B_ECHO);
	if (!ft_strcmp(p->cmd, "setenv"))
		return (p->setup |= B_SETENV);
	if (!ft_strcmp(p->cmd, "unsetenv"))
		return (p->setup |= B_UNSETENV);
	if (!ft_strcmp(p->cmd, "env"))
		return (p->setup |= B_ENV);
	if (!ft_strcmp(p->cmd, "cd"))
		return (p->setup |= B_CD);
	if (!ft_strcmp(p->cmd, "exit"))
		return (p->setup |= B_EXIT);
	if (!ft_strcmp(p->cmd, "hash"))
		return (p->setup |= B_HASH);
	if (!ft_strcmp(p->cmd, "jobs"))
		return (p->setup |= B_JOBS);
	if (!ft_strcmp(p->cmd, "bg"))
		return (p->setup |= B_BG);
	if (!ft_strcmp(p->cmd, "fg"))
		return (p->setup |= B_FG);
	if (!ft_strcmp(p->cmd, "type"))
		return (p->setup |= B_TYPE);
	return (0);
}

uint8_t				find_binary(t_list *env, t_process *p, t_cfg *shell)
{
	if ((p->path = ft_strdup(ft_hash_lookup(shell->map, p->cmd))))
	{
		if (!(ERROR & path_errors(p->path, 1)))
			return (TRUE);
		ft_strdel(&p->path);
		ft_hash_delone(shell->map, p->cmd, free);
		if ((p->path = ft_which(find_var_value(env, "PATH"), p->cmd)))
			ft_hash_add(shell->map, p->cmd, p->path, ft_strlen(p->path) + 1);
		return (p->path ? 1 : 0);
	}
	if (!(p->path = ft_which(find_var_value(env, "PATH"), p->cmd)))
		return (0);
	ft_hash_add(shell->map, p->cmd, p->path, ft_strlen(p->path) + 1);
	return (TRUE);
}

static uint16_t		find_type(t_list *env, t_process *p)
{
	if (builtin_search(p))
		p->setup |= BUILTIN;
	else if (find_binary(env, p, cfg_shell()))
		p->setup |= EXEC;
	else
		return (p->setup |= E_UNFOUND);
	return (SUCCESS);
}

static void			any_slash(t_list *env, t_process *p)
{
	if (find_type(env, p) || p->setup & BUILTIN)
		return ;
	p->setup |= path_errors(p->path, 1);
}

void			with_slash(t_process *p)
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

void				process_type(t_process *p)
{
	if (ft_strchr(p->cmd, '/'))
		with_slash(p);
	else (any_slash(p->env, p));
}
