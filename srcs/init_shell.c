#include "exec.h"
#include "sh.h"
#include "struct.h"
#include "libft.h"
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>
#include <signal.h>

struct termios *term_create_origin(void)
{
	struct termios *term;

	if (!(term = malloc(sizeof(struct termios))))
		ex("[TERM ORIGIN] ERROR MALLOC");
	if ((tcgetattr(STDIN_FILENO, term) == FALSE))
		ex("[TERM ORIGIN] ERROR TCGETATTR");
	return (term);
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


void		init_shell(char **env, char **av)
{
	uint8_t	shell_terminal;
	pid_t	shell_pgid;
	t_cfg	*shell;

	shell_terminal = STDIN_FILENO;
	shell = init_cfg(env, av);
	if ((shell->interactive = isatty(shell_terminal)))
	{
		while (tcgetpgrp(shell_terminal) != (shell_pgid = getpgrp()))
			kill(-shell_pgid, SIGTTIN);
		set_signal_ign();
		if (setpgid(shell->pid, shell->pid) < 0)
			ex("[INIT SHELL] error setpgid");
		if (tcsetpgrp(STDIN_FILENO, shell->pid))
			ex("[INIT SHELL] error tcsetpgrp");
		shell->term_origin = term_create_origin();
	}
	//else
}
