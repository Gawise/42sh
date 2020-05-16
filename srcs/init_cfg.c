#include "struct.h"
#include "libft.h"
#include "sh.h"
#include "var.h"
#include "ft_printf.h"
#include "line_edition.h"

static void		set_var_sp(t_cfg *shell)
{
	char	*pid;

	pid = ft_itoa(shell->pid);
	setvar_add(&shell->sp, "$", pid);
	setvar_add(&shell->sp, "?", "0");
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

static void		debug_path_fail(char *path)
{
	char		*taberr[6];
	uint32_t	err;
	
	taberr[0] = STR_UNFOUND;
	taberr[1] = STR_ISDIR;
	taberr[2] = STR_NOENT;
	taberr[3] = STR_ACCES;
	taberr[4] = STR_LOOP;
	taberr[5] = STR_NTL;
	if (!(err = path_errors(path, TRUE)))
		ft_ex(USAGE);
	ft_dprintf(STDERR_FILENO, "%s; Debug option fail: %s: %s\n", PROJECT, path, tab[err >> 7]);
	ft_ex(USAGE);
}

static uint8_t		set_debug(char **av, int i)
{
	int		fd;
	
	if (!av[i])
		return (STDERR_FILENO);
	if ((fd = open(av[i], O_CREAT | O_WRONLY, 0644)) == -1) /* check_path */
		debug_path_fail(av[i]);
	return (fd);
}

static void		set_shell_mode(char **av, int ac, t_cfg *shell)
{
	int		i;

	(void)ac;
	i = 1;
	shell->interactive = 1;
	if (av[i] && av[i][0] == '-') /*get opt*/
	{
		if (ft_strequ("-d", av[i++]))
			shell->debug = set_debug(av, i++);
		else
			ft_ex(USAGE);
	}
	if (av[i] && !(shell->file = ft_strdup(av[i])))
		ft_ex(EXMALLOC);
	if (shell->file)
		shell->interactive = 0;
}

t_cfg			*init_cfg(char **env, char **av, int ac)
{
	t_cfg	*shell;

	shell = cfg_shell();
	ft_bzero(shell, sizeof(t_cfg));
	shell->pid = getpid();
	create_lst_var(&shell->env, env);
	set_var_intern(shell);
	set_var_sp(shell);
	shell->history = get_history();
	if (!(shell->map = ft_hash_init(128))
		|| !(shell->input_map = ft_hash_init(21)))
		ft_ex(EXMALLOC);
	init_input_map(shell->input_map);
	set_shell_mode(av, ac, shell);
	return (shell);
}
