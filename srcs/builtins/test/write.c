
#include "libft.h"
#include "struct.h"
#include "sh.h"
#include "ft_printf.h"
#include "var.h"

/*
 *	test -w pathname
 *	0 si pathname est un fichier avec write premission
 */ 


uint8_t		test_write_file(char *path)
{
	struct stat buf;

	if (lstat(path, &buf))
		return (failure);
	buf.st_mode & S_IWUSR ? return (0) : return (1);
}
