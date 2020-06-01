#include "libft.h"
#include "struct.h"
#include "sh.h"
#include "ft_printf.h"
#include "var.h"

/*
 *	test -d pathname
 *	verifie que pathname est un directory
 */

uint8_t		test_dir_file(char *path)
{
	struct stat buf;

	if (lstat(path, &buf))
		return (failure);
	buf.st_mode & S_IFDIR ? return (0) : return (1);
}
