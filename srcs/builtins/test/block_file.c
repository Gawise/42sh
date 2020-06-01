#include "libft.h"
#include "struct.h"
#include "sh.h"
#include "ft_printf.h"
#include "var.h"

/* 
 *  	test -b pathname
 *	verifie que le pathname est un block file
 */

uint8_t		test_bloc_file(char *path)
{
	struct stat buf;

	if (lstat(path, &buf))
		return (failure);
	buf.st_mode & S_IFBLK ? return (0) : return (1);
}
