#include "libft.h"
#include "struct.h"
#include "sh.h"
#include "ft_printf.h"
#include "var.h"

/*
 *	test -x pathname
 *	pathname est un fichier avec les permissions d'execution
 */

uint8_t		test_exec_file(char *path)
{
	struct stat buf;

	if (lstat(path, &buf))
		return (failure);
	buf.st_mode & S_IXUSR ? return (0) : return (1);


}
