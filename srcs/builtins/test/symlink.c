
#include "libft.h"
#include "struct.h"
#include "sh.h"
#include "ft_printf.h"
#include "var.h"

/*
 *	test -L pathname (ou -h)
 *	0 si pathname est un symlink
 */

uint8_t		test_symlink_file(char *path)
{
	struct stat buf;

	if (stat(path, &buf))
		return (failure);
	buf.st_mode & S_IFLNK ? return (0) : return (1);
}
