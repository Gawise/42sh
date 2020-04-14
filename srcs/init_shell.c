#include "exec.h"
#include "var.h"
#include "sh.h"
#include "struct.h"
#include "libft.h"
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>

static int	check_terminal(t_cfg *cfg, uint8_t tty)
{
	if (cfg && (cfg->interactive = isatty(tty)))
	{
		if ((tcgetattr(tty, &cfg->term_origin) == FALSE))
			perror("[TERM ORIGIN] ERROR TCGETATTR"); ///perror
		return (1);
	}
	return (0);
}

void		set_signal_ign(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
	signal(SIGTTIN, SIG_IGN);
	signal(SIGTTOU, SIG_IGN);
//	signal(SIGCHLD, SIG_IGN); //for job control
}

t_cfg		*cfg_shell(void)
{
	static t_cfg shell;

	return (&shell);
}


void		init_shell(char **env, char **av, int ac)
{
	uint8_t		shell_terminal;
	pid_t		shell_pgid;
	t_cfg		*shell;
	struct stat	stat;

	if (fstat((shell_terminal = ttyslot()), &stat) == -1)
		ft_ex("file descriptor stdin close\nbad file descriptor\nexit");
	shell = init_cfg(env, av, ac);
	if (check_terminal(shell, shell_terminal))
	{
		while (tcgetpgrp(shell_terminal) != (shell_pgid = getpgrp()))
			kill(-shell_pgid, SIGTTIN);
		set_signal_ign();
		if (setpgid(shell->pid, shell->pid) < 0)
			perror("[INIT SHELL] error setpgid");
		if (tcsetpgrp(shell_terminal, shell->pid))
			perror("[INIT SHELL] error tcsetpgrp");
	}
}
