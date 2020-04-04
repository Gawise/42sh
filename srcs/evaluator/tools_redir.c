#include "exec.h"
#include "sh.h"
#include <unistd.h>

void	do_my_dup2(int8_t fd1, int8_t fd2)
{
	if (fd1 == -1)
		return ;
	if (dup2(fd1, fd2) == -1)
		ft_ex("Fatal Error DUP2\nexit\n");
}
