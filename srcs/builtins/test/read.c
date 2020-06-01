
#include "libft.h"
#include "struct.h"
#include "sh.h"
#include "ft_printf.h"
#include "var.h"

/*
 *	test -r pathname
 *	0 si pathname est un fichier avec read permissions
 */

uint8_t		test_read_file(char *path)
{
	struct stat buf;

	if (lstat(path, &buf))
		return (failure);
	buf.st_mode & S_IRUSR ? return (0) : return (1);
}
