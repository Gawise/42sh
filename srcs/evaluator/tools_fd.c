#include "sh.h"
#include "exec.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

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

uint32_t	right_fifo(char *path, uint32_t *right)
{
	struct stat buf;

	lstat(path, &buf);
	if (!S_ISFIFO(buf.st_mode))
		return (0);
	if (access(path, R_OK | W_OK) != SUCCESS)
		return (E_ACCES);
	*right = O_RDWR;
	return (SUCCESS);
}
