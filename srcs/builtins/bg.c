#include "ft_printf.h"
#include "struct.h"

uint8_t		ft_bg(t_job *j, t_process *p)
{
	(void)j;
	(void)p;
	ft_dprintf(STDERR_FILENO, "Builtin bg not enable\n");
	return (1);
}
