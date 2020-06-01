#include "sh.h"
#include <sys/types.h>
#include <sys/stat.h>

uint8_t		test_setuid_file(char *path)
{
	struct stat buf;

	if (lstat(path, &buf))
		return (FAILURE);
	buf.st_mode & S_ISUID ? return (0) : return (1);
}

