#include "libft.h"
#include "exec.h"
#include <termios.h>
#include <unistd.h>

void		set_termios(struct termios *term)
{
	if (tcsetattr(STDIN_FILENO, TCSADRAIN, term) == -1)
		perror("[RUN JOB] error tcsetattr");
}

void	term_create_eval(struct termios *origin, struct termios *eval)
{
	ft_memcpy(eval, origin, sizeof(struct termios));
	eval->c_lflag &= ~TOSTOP;
}
