#include "sh.h"
#include "ft_printf.h"
#include <stdlib.h>

void	ft_ex(char *error)
{
	ft_dprintf(STDERR_FILENO,"%s: %s", PROJECT, error);
	exit(EXIT_FAILURE);
}

void	protect_malloc(void *malloc)
{
	if (!malloc)
		ft_ex(EXMALLOC);
}

