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
		return ((p->setup |= B_ECHO) + 1);
	if (!ft_strcmp(p->cmd, "set"))
		return (p->setup |= B_SET);
	if (!ft_strcmp(p->cmd, "unset"))
		return (p->setup |= B_UNSET);
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
	if (!ft_strcmp(p->cmd, "export"))
		return (p->setup |= B_EXPORT);
	if (!ft_strcmp(p->cmd, "alias"))
		return (p->setup |= B_ALIAS);


	return (0);
}

static uint8_t		find_binary(t_cfg *shell, t_process *p, t_list *env)
{
	char	*var_path;

	if (!(var_path = find_var_value(env, "PATH")))
		var_path = find_var_value(shell->intern, "PATH");
	if (!var_path)
	{
		ft_hash_reset(&cfg_shell()->map, free, 128);
		return (0);
	}
	if ((p->path = ft_strdup(ft_hash_lookup(shell->map, p->cmd))))
	{
		if (!(ERROR & path_errors(p->path, 1, S_IXUSR)))
			return (TRUE);
		ft_strdel(&p->path);
		ft_hash_delone(shell->map, p->cmd, free);
		if ((p->path = ft_which(var_path, p->cmd)))
			ft_hash_add(shell->map, p->cmd, ft_strdup(p->path), free);
		return (p->path ? 1 : 0);
	}
	if (!(p->path = ft_which(var_path, p->cmd)))
		return (0);
	ft_hash_add(shell->map, p->cmd, ft_strdup(p->path), free);
	return (TRUE);
}

static uint16_t		find_type(t_list *env, t_process *p, uint32_t *err)
{
	if (builtin_search(p))
		p->setup |= BUILTIN;
	else if (find_binary(cfg_shell(), p, env))
		p->setup |= EXEC;
	else
		return (*err |= E_UNFOUND);
	return (SUCCESS);
}

void				any_slash(t_list *env, t_process *p, uint32_t *err)
{
	if (find_type(env, p, err) || p->setup & BUILTIN)
		return ;
	*err |= path_errors(p->path, 1, S_IXUSR);
}

void				with_slash(t_process *p, uint32_t *err)
{
	char		*tmp;

	p->setup |= EXEC;
	p->path = ft_strdup(p->cmd);
	protect_malloc(p->path);
	if (*p->path == '/')
		*err |= path_errors(p->path, 1, S_IXUSR);
	else
	{
		tmp = create_abs_path(p->path);
		*err |= path_errors(tmp, 1, S_IXUSR);
		ft_strdel(&tmp);
	}
}
