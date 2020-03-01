#include "struct.h"
#include "libft.h"
#include "sh.h"
#include "var.h"



static void	set_readonly(t_list *lst)
{
	int		i;
	t_var	*var;

	i = 0;
	while (i++ < 9)
	{
		var = lst->data;
		var->rd = 1;
		lst = lst->next;
	}
}

static void	set_var_intern(t_cfg *shell)
{
	char	*pid;

	pid = ft_itoa(shell->pid);
	ft_setvar(&shell->intern, "?", "0", 1);
	ft_setvar(&shell->intern, "$", pid, 1);
	ft_setvar(&shell->intern, "!", "0", 1);
	ft_setvar(&shell->intern, "*", 0, 1);
	ft_setvar(&shell->intern, "@", 0, 1);
	ft_setvar(&shell->intern, "#", 0, 1);
	ft_setvar(&shell->intern, "-", 0, 1);
	ft_setvar(&shell->intern, "0", 0, 1);
	ft_setvar(&shell->intern, "_", 0, 1);
	ft_setvar(&shell->intern, "PS1", NAME_SH, 1);
	ft_setvar(&shell->intern, "PS2", "> ", 1);
	set_readonly(shell->intern);
	ft_strdel(&pid);
}

static uint8_t		set_debug(char **av)
{
	int i;

	i = 1;
	while (av[i])
		if (!ft_strcmp(av[i++], "debug"))
			return (1);
	return (0);
}


t_cfg		*init_cfg(char **env, char **av)
{
	t_cfg	*shell;

	shell = cfg_shell();
	ft_bzero(shell, sizeof(t_cfg));
	shell->pid = getpid();
	create_lst_var(&shell->env, env);
	set_var_intern(shell);
	shell->debug = set_debug(av);
	return (shell);
}

