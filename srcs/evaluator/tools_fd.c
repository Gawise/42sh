#include "sh.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int32_t		create_fd_null(void)
{
	int32_t	fd;

	if ((fd = open("/dev/null", 0)) == -1)
		if ((fd = open("/tmp/tmpfd2142sh", O_CREAT, 0644)) == -1)
			return (0);
	return (fd);
}

uint8_t		bad_fd(int fd)
{
	struct stat buf;

	if (fstat(fd, &buf) == FALSE)
		return (FAILURE);
	return (SUCCESS);
}
