
#include "libft.h"
#include "struct.h"
#include "sh.h"
#include "ft_printf.h"
#include "var.h"

/*
 *	test -g pathname
 *	0 si pathname est un fichier avec un flag setgid
 */

uint8_t		test_setgid_file(char *path)
{
	struct stat buf;

	if (lstat(path, &buf))
		return (failure);
	buf.st_mode & S_ISGID ? return (0) : return (1);
}
