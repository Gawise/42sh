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
	ft_setvar(&shell->intern, "PS1", NAME_SH);
	ft_setvar(&shell->intern, "PS2", "> ");
}

static void		argv_path_fail(char *path, char *mode, uint32_t err)
{
	char		*taberr[6];
	int8_t		tmp;
	
	tmp = 0;
	taberr[0] = STR_ISDIR;
	taberr[1] = STR_NOENT;
	taberr[2] = STR_ACCES;
	taberr[3] = STR_LOOP;
	taberr[4] = STR_NTL;
	if (!err)
		ft_ex(USAGE);
	err = err >> 7;
	while (!((err = err >> 1) & 1))
		tmp++;
	ft_dprintf(STDERR_FILENO, "%s; %s: %s: %s\n", PROJECT, mode, path, taberr[tmp]);
	ft_ex(USAGE);
}

static uint8_t		set_debug(char **av, int *ac)
{
	int			fd;
	uint32_t	err;
	
	if (!av[*ac] || *av[*ac] == '-')
		return (STDERR_FILENO);

	if ((fd = open(av[*ac], O_CREAT | O_WRONLY | O_APPEND, 0644)) == -1)
	{
		err = path_errors(av[*ac], TRUE, S_IWUSR);
		argv_path_fail(av[*ac], "Debug mode fail", err);
	}
	*ac += 1;
	return (fd);
}

static void		set_shell_mode(char **av, int ac, t_cfg *shell)
{
	int32_t		 i;
	int8_t		ret;

	i = 0;
	shell->interactive = TRUE;
	if (ac == 1)
		return ;
	ac = 1;
	while ((ret = ft_getopt(&ac, &i, av, "d")) != -1)
	{
		if (ret == '?')
		{
			ft_dprintf(STDERR_FILENO, "%s: -%c : Bad option\n", PROJECT, av[ac][i]);
			ft_ex(USAGE);
		}
		else if (ret == 'd')
			shell->debug = set_debug(av, &ac);
	}
	if (av[ac] && !(shell->file = ft_strdup(av[ac])))
		ft_ex(EXMALLOC);
	if (shell->file && !((i = path_errors(shell->file, TRUE, S_IRUSR))))
		shell->interactive = 0;
	else if (i)
		argv_path_fail(shell->file, "Non-interactive mode fail", i);
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
