#include "exec.h"
#include "sh.h"
#include "libft.h"
#include <unistd.h>
#include <sys/stat.h>

void	do_my_dup2(int8_t fd1, int8_t fd2)
{
	if (fd1 == -1)
		return ;
	if (dup2(fd1, fd2) == -1)
		ft_ex("Fatal Error DUP2\nexit\n");
}

uint8_t		bad_fd(int fd)
{
	struct stat buf;

	if (fstat(fd, &buf) == FALSE)
		return (FAILURE);
	return (SUCCESS);
}

uint32_t	path_gearing(t_redir *r, char **path, int right)
{
	int32_t		exist;
	int32_t		r_or_w;

	r_or_w = (right & O_WRONLY) ? W_OK : R_OK;
	*path = (*r->file == '/') ? ft_strdup(r->file) : create_abs_path(r->file);
	if (((exist = check_access(*path, r_or_w)) & (E_ACCES | E_NOENT | E_ISDIR)))
		return (exist);
	if (r->type == LESS && !exist)
		return (E_NOENT);
	return (SUCCESS);
}
