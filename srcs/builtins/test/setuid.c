
#include "libft.h"
#include "struct.h"
#include "sh.h"
#include "ft_printf.h"
#include "var.h"

/*
 *	test -u pathname
 *	0 si pathname est un fichier avec un flag setuid
 */ 

uint8_t		test_setuid_file(char *path)
{
	struct stat buf;

	if (lstat(path, &buf))
		return (failure);
	buf.st_mode & S_ISUID ? return (0) : return (1);
}

