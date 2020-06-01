#include "libft.h"
#include "struct.h"
#include "sh.h"
#include "ft_printf.h"
#include "var.h"

/*
 *	test -p pathname
 *	verfie que pathname est un FIFO
 */

uint8_t		test_fifo_file(char *path)
{
	struct stat buf;

	if (lstat(path, &buf))
		return (failure);
	buf.st_mode & S_IFIFO ? return (0) : return (1);
}
