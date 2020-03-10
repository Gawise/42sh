#include <signal.h>
#include <term.h>
#include <termios.h>
#include <sys/ioctl.h>
#include "line_edition.h"
#include <unistd.h>
#include <stdlib.h>
#include "libft.h"
#include "struct.h"
#include "sh.h"

void	size_handler(int sig)
{
	t_cs_line	*cs;
	int			cr;

	if (sig == SIGWINCH && (cs = cs_master(NULL, 0)))
	{
		cs->min_row = 0;
		cs->min_col = 0;
		ft_clear(1);
		cs_set();
		cr = get_line(cs);
		cs->cr = cr;
		cs->scroll = cs->cr - (cs->screen.y - cs->min_row - 1);
		if (cs->scroll < 0)
			cs->scroll = 0;
		print_cmdline(cs);
	}
}

void	sig_handler(int sig)
{
	t_cs_line	*cs;

	if (sig >= 0 && (cs = cs_master(NULL, 0)))
	{
		cs->sig_int = 1;
		end_key(cs);
		ft_strdel(&cs->input);
		if (cs->history)
		{
			cs->history->data = NULL;
			ft_dlstdel(&cs->history);
			cs->input = NULL;
			cs->history = NULL;
		}
	}
}

void	init_signals(void)
{
	int	i;

	i = 0;
	while (i <= 32)
	{
		if (i == SIGCONT || i == SIGTSTP || i == SIGSTOP || i == SIGTTOU
				|| i == SIGTTIN)
			signal(i, SIG_IGN);
		else if (i == SIGWINCH)
			signal(i, size_handler);
		i++;
	}
	signal(SIGINT, sig_handler);
}
