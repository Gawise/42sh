
#include "libft.h"
#include "struct.h"
#include "sh.h"
#include "ft_printf.h"
#include "var.h"

/*
 *	test -S pathname
 *	0 si pathname est une socket
 */

uint8_t		test_socket_file(char *path)
{
	struct stat buf;

	if (lstat(path, &buf))
		return (failure);
	buf.st_mode & S_IFSOCK ? return (0) : return (1);
}
