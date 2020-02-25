#include "exec.h"
#include "sh.h"
#include "struct.h"
#include "libft.h"
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>
#include <signal.h>

void		set_signal_ign(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
	signal(SIGTTIN, SIG_IGN);
	signal(SIGTTOU, SIG_IGN);
	signal(SIGCHLD, SIG_DFL);  //only for exec ?
	//signal(SIGCHLD, SIG_IGN);
}

void		init_cfg(t_cfg *cfg, char **env)
{
	ft_bzero(cfg, sizeof(t_cfg));
	cfg->pid = getpid();
	create_lst_env(&cfg->var, env);
	// faire liste de variables interne ??
}

void		init_shell(t_cfg *cfg, char **env)
{
	uint8_t	shell_terminal;
	pid_t	shell_pgid;

	shell_terminal = STDIN_FILENO;
	init_cfg(cfg, env);
	if ((cfg->interactive = isatty(shell_terminal)))
	{
		while (tcgetpgrp(shell_terminal) != (shell_pgid = getpgrp()))
			kill(-shell_pgid, SIGTTIN);
		set_signal_ign();
		if (setpgid(cfg->pid, cfg->pid) < 0)
			ex("[INIT SHELL] error setpgid");
		if (tcsetpgrp(STDIN_FILENO, cfg->pid))
			perror("[INIT SHELL] error tcsetpgrp");
		tcgetattr(STDIN_FILENO, &cfg->term_origin);
	}

// faire partie non interactive !!

}
