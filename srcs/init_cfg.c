#include "struct.h"
#include "libft.h"
#include "sh.h"
#include "var.h"
#include "ft_printf.h"


static void		set_var_sp(t_cfg *shell)
{
	char	*pid;

	pid = ft_itoa(shell->pid);
	setvar_add(&shell->sp, "$", pid);
	setvar_add(&shell->sp, "?", "13");
	setvar_add(&shell->sp, "!", "0");
	setvar_add(&shell->sp, "*", 0);
	setvar_add(&shell->sp, "@", 0);
	setvar_add(&shell->sp, "#", 0);
	setvar_add(&shell->sp, "-", 0);
	setvar_add(&shell->sp, "0", 0);
	ft_strdel(&pid);
}

static void		set_var_intern(t_cfg *shell)
{
	ft_setvar(&shell->intern, "_", 0);
	ft_setvar(&shell->intern, "PS1", NAME_SH);
	ft_setvar(&shell->intern, "PS2", "> ");
}

static uint8_t		set_debug(char **av)
{
	int 	i;
	int		fd;
	char	*path;

	i = 0;
	while (i++ && av[i])
		if (!ft_strcmp(av[i], "debug"))
			break ;
	if (!av[i])
		return (0);
	if (!av[i + 1] || *av[i + 1] != '-')
		return (1);
	path = (av[i + 1]) + 1;
	if ((fd = open(path, O_CREAT | O_WRONLY, 0644)) == -1)
	{
		ft_dprintf(2, "Debug option error\n\tusage: ./21sh debug [-path]\nexit\n");
		exit(1);
	}
	return (fd);
}

t_cfg			*init_cfg(char **env, char **av, int ac)
{
	t_cfg	*shell;

	shell = cfg_shell();
	ft_bzero(shell, sizeof(t_cfg));
	shell->pid = getpid();
	create_lst_var(&shell->env, env);
	ft_setvar(&shell->env, "PROJECT", "21sh");
	set_var_intern(shell);
	set_var_sp(shell);
	if (!(shell->map = ft_hash_init(128)))
		ft_ex(EXMALLOC);
	if (ac > 1)
		shell->debug = set_debug(av);
	return (shell);
}
