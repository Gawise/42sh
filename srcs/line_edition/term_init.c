#include <signal.h>
#include <termios.h>
#include <term.h>
#include "line_edition.h"
#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdlib.h>
#include "libft.h"
#include "struct.h"

void	set_term(int tty, int init, char *prompt, struct termios *new_term)
{
	t_cs_line			*cs;

	signal(SIGINT, sig_handler);
	(*new_term).c_lflag &= ~(ICANON);
	(*new_term).c_lflag &= ~ECHO;
	(*new_term).c_cc[VMIN] = 1;
	(*new_term).c_cc[VTIME] = 0;
	tcsetattr(tty, TCSADRAIN, new_term);
	cs_master(prompt, 1);
	cs = cs_master(NULL, 0);
	if (init == 2 && cs)
		get_cs_line_position(&cs->min_col, &cs->min_row);
	cs_set();
	cs->tty = tty;
	move_cs(&cs);
}

void	unset_term(struct termios *old_term)
{
	t_cs_line	*cs;

	if ((cs = cs_master(NULL, 0)))
	{
		tcsetattr(cs->tty, 0, old_term);
	}
}

int		term_check(struct termios *new_term, struct termios *old_term, int tty)
{
	int ret;

	ret = 0;
	if ((ret = 1))
	{
		if (!isatty(tty) && !(ret = 0))
			ft_putstr_fd("ft_select: Not a valid terminal type device\n", 2);
		if (ret && !getenv("TERM") && !(ret = 0))
			ft_putstr_fd("ft_select: Unvalid environnement\n", 2);
		if (ret && tgetent(NULL, getenv("TERM")) == 0 && !(ret = 0))
		{
			ft_putstr_fd("ft_select: Could not retrieve terminal", 2);
			ft_putstr_fd(" in terminfo database\n", 2);
		}
		else if (ret && tgetent(NULL, getenv("TERM")) == -1 && !(ret = 0))
			ft_putstr_fd("ft_select: Terminfo database not found\n", 2);
		if (ret && (tcgetattr(tty, old_term) == -1
					|| tcgetattr(tty, new_term) == -1) && !(ret = 0))
			ft_putstr_fd("ft_select: Could not get terminal attributes\n", 2);
		init_signals();
	}
	return (ret);
}

int		term_init(int init, char *prompt)
{
	struct termios			new_term;
	static struct termios	old_term;
	int						tty;

	tty = ttyslot();
	if (init >= 1 && term_check(&new_term, &old_term, tty) == 1)
	{
		set_term(tty, init, prompt, &new_term);
		return (1);
	}
	if (init == 0)
	{
		unset_term(&old_term);
		return (1);
	}
	return (0);
}
