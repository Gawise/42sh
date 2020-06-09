#include "exec.h"
#include "var.h"
#include "sh.h"
#include "struct.h"
#include "libft.h"
#include "ft_printf.h"
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>
#include <sys/stat.h>
#include <signal.h>

void		set_var(t_cfg *shell)
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
	ft_setvar(&shell->intern, "PS1", NAME_SH);
	ft_setvar(&shell->intern, "PS2", "> ");
}

void		hdl_sighup(int sig)
{
	t_cfg	*shell;
	t_list	*lst;
	t_job	*j;

	shell = cfg_shell();
	lst = shell->job;
	while (lst)
	{
		j = lst->data;
		kill(-j->pgid, sig);
		kill(-j->pgid, SIGCONT);
		lst = lst->next;
	}
	if (shell->cur_job)
		kill(-shell->cur_job, sig);
	ft_printf("\n%s:\t%d hangup\n", PROJECT, shell->pid);
	exit_routine(shell, 129);
}

static int	check_terminal(t_cfg *cfg, uint8_t tty)
{
	if (isatty(tty))
	{
		if ((tcgetattr(tty, &cfg->term_origin) == FALSE))
			ft_ex(EX);
		return (1);
	}
	ft_ex(EXFD);
	return (0);
}

t_cfg		*cfg_shell(void)
{
	static t_cfg shell;

	return (&shell);
}

t_cfg		*init_shell(char **env, char **av, int ac)
{
	uint8_t		shell_terminal;
	pid_t		shell_pgid;
	t_cfg		*shell;

	shell_terminal = STDIN_FILENO;
	shell = init_cfg(env, av, ac);
	if (shell->interactive && check_terminal(shell, shell_terminal))
	{
		while (tcgetpgrp(shell_terminal) != (shell_pgid = getpgrp()))
			kill(-shell_pgid, SIGTTIN);
		set_signal_ign();
		signal(SIGHUP, hdl_sighup);
		if (setpgid(shell->pid, shell->pid) < 0)
			ft_ex(EX);
		if (tcsetpgrp(shell_terminal, shell->pid))
			ft_ex(EX);
	}
	return (shell);
}
