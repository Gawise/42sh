#include "exec.h"
#include "sh.h"
#include "libft.h"
#include <unistd.h>
#include <sys/stat.h>

void		add_fd_process(t_list **fd, int16_t source, int16_t target)
{
	int16_t		tab[2];

	tab[0] = source;
	tab[1] = target;
	ft_lst_push_back(fd, tab, sizeof(tab));
}

void		do_my_dup2(int16_t fd1, int16_t fd2)
{
	if (fd1 == -1)
		return ;
	if (dup2(fd1, fd2) == -1)
		ft_ex("Fatal Error DUP2\nexit\n");
}

void		do_redir(t_list *fd)
{
	int16_t *t;

	while (fd)
	{
		t = fd->data;
		if (t[0] == -1)
			close(t[1]);
		else if (t[0] != t[1])
			do_my_dup2(t[0], t[1]);
		fd = fd->next;
	}
}

void		do_pipe(t_process *p)
{
	if (p->std[0] != STDIN_FILENO &&
			p->std[0] != -1)
	{
		do_my_dup2(p->std[0], STDIN_FILENO);
		if (close(p->std[0]) == -1)
			ft_ex("[do_pipe]1 close error:"); //debug
	}
	if (p->std[1] != STDOUT_FILENO &&
			p->std[1] != -1)
	{
		do_my_dup2(p->std[1], STDOUT_FILENO);
		if (close(p->std[1]) == -1)
			ft_ex("[do_pipe]2 close error:"); //debug
	}
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
