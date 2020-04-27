#include "struct.h"
#include "libft.h"
#include "sh.h"
#include "var.h"
#include "ft_printf.h"

static void		set_var_intern(t_cfg *shell)
{
	char	*pid;

	pid = ft_itoa(shell->pid);
	ft_setvar(&shell->intern, "?", "0");
	ft_setvar(&shell->intern, "$", pid);
	ft_setvar(&shell->intern, "!", "0");
	ft_setvar(&shell->intern, "*", 0);
	ft_setvar(&shell->intern, "@", 0);
	ft_setvar(&shell->intern, "#", 0);
	ft_setvar(&shell->intern, "-", 0);
	ft_setvar(&shell->intern, "0", 0);
	ft_setvar(&shell->intern, "_", 0);
	ft_setvar(&shell->intern, "PS1", NAME_SH);
	ft_setvar(&shell->intern, "PS2", "> ");
	ft_strdel(&pid);
}

static uint8_t		set_debug(char **av, int i)
{
	int		fd;
	char	*path;

	if (!(path = av[i])
	|| (fd = open(path, O_CREAT | O_WRONLY, 0644)) == -1)
	{
		ft_dprintf(2, "Usage: %s [-d path] file\nexit\n", NAME_SH);
		exit(1);
	}
	return (fd);
}

static void		set_shell_mode(char **av, int ac, t_cfg *shell)
{
	int		i;

	(void)ac;
	i = 1;
	shell->mode = INTERACTIVE_MODE;
	if (av[i] && av[i][0] == '-')
	{
		if (ft_strequ("-d", av[i++]))
			shell->debug = set_debug(av, i++);
		else
		{
			ft_dprintf(2, "Usage: %s [-d path] file\nexit\n", NAME_SH);
			exit(1);
		}
	}
	if (av[i] && !(shell->file = ft_strdup(av[i])))
		ft_ex(EXMALLOC);
	if (shell->file)
		shell->mode = NON_INTERACTIVE_MODE;
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
	if (!(shell->map = ft_hash_init(128)))
		ft_ex(EXMALLOC);
	set_shell_mode(av, ac, shell);
	return (shell);
}
