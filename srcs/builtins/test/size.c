
#include "libft.h"
#include "struct.h"
#include "sh.h"
#include "ft_printf.h"
#include "var.h"

/*
 *	test -s pathname
 *	0 si pathname est un fichier de taille superieur a 0
 */

uint8_t		test_size_file(char *path)
{
	struct stat buf;

	if (lstat(path, &buf))
		return (failure);
	buf.st_size > 0 ? return (0) : return (1);
}
