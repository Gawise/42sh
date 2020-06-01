
#include "libft.h"
#include "struct.h"
#include "sh.h"
#include "ft_printf.h"
#include "var.h"

/*
 *	test -e pathname
 *	verifie que pathname est un file (pas un regaular file)
 */

uint8_t		test_bloc_file(char *path)
{
	struct stat buf;

	if (lstat(path, &buf))
		return (failure);
	return (SUCCESS);
}
