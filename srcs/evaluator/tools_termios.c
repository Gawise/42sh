#include "libft.h"
#include "exec.h"
#include "sh.h"

void	set_termios(int32_t mode, struct termios *term)
{
	if (tcsetattr(STDIN_FILENO, mode, term) == -1)
		ft_ex(EXUEPTD);
}

void	term_create_eval(struct termios *origin, struct termios *eval)
{
	ft_memcpy(eval, origin, sizeof(struct termios));
	eval->c_lflag &= ~TOSTOP;
}
