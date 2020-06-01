
#include "libft.h"
#include "struct.h"
#include "sh.h"
#include "ft_printf.h"
#include "var.h"

/*
 *	test -f pathname
 *	0 si pathname est un regular file
 */

uint8_t		test_regular_file(char *path)
{
	struct stat buf;

	if (lstat(path, &buf))
		return (failure);
	buf.st_mode & S_IFREG ? return (0) : return (1);
}
