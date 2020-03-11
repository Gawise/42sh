#include "exec.h"
#include "libft.h"
#include "struct.h"
#include "ft_printf.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

//						lsof -a -p $$ -d0,1,2
//  cmd1 3>&- | cmd2 2>&3 3>&- | 2>&1 >&4 4>&- | cmd3 3>&- 4>&-



/*
 *typedef struct			s_redir
 *{
 *    char			*delim;
 *    char			*io_num;
 *    t_token_type	type; // enum
 *    char			*file;
 *}				t_redir;
 */

/*
   O_RDONLY        open for reading only
   O_WRONLY        open for writing only
   O_RDWR          open for reading and writing
   O_NONBLOCK      do not block on open or for data to become available
   O_APPEND        append on each write
   O_CREAT         create file if it does not exist
   O_TRUNC         truncate size to 0
   O_EXCL          error if O_CREAT and the file exists
   O_SHLOCK        atomically obtain a shared lock
   O_EXLOCK        atomically obtain an exclusive lock
   O_NOFOLLOW      do not follow symlinks
   O_SYMLINK       allow open of symlinks
   O_EVTONLY       descriptor requested for event notifications only
   O_CLOEXEC       mark as close-on-exec
   */
/*
 *# define ERROR 64
 *# define E_UNFOUND 192
 *# define E_ISDIR 320
 *# define E_NOENT 576
 *# define E_ACCES 1088
 *# define E_LOOP 2112
 *# define E_NTL	4160	
 */

/*
   rdonly 0
   wronly 1
   creat 512
   append 8
   trunc 1024
   r_ok 4
   w_ok 2
 *
 */ 
/*
 *    LESS,			// <
 *    DLESS,			// <<
 *    GREAT,			// >
 *    DGREAT,			// >>
 *    LESSAND,		// <&
 *    GREATAND,		// >&
 *    DLESSDASH,		// <<-
 *}				t_token_type;
 */

void		debug(int i)
{
	printf("retour erreur redir = [%d]\n", i);
}

char		*remove_file_name(char *s)
{
	int		i;
	char	*dst;

	dst = ft_strdup(s);
	i = ft_strrchri(dst, '/');
	dst[i + 1] = '\0'; /// possible leaks ?
	return (dst);
}

int		check_right(char *path, int right)
{
	if (access(path, right))
		return (FALSE);
	return (SUCCESS);
}

int		check_access(char *path, int right)
{
	char	*tmp;

	tmp = NULL;
	if (access(path, F_OK) == SUCCESS)
	{
		if (c_isdir(path))
			return (E_ISDIR);
		if (check_right(path, right))
			return (E_ACCES);
		return (TRUE);
	}
	printf("check access 2 \n");
	tmp = remove_file_name(path);
	if (access(tmp, F_OK))
	{
		ft_strdel(&tmp);
		return (E_NOENT);
	}
	if (access(tmp, right))
	{
		ft_strdel(&tmp);
		return (E_ACCES);
	}
	return (SUCCESS);
}

int		path_gearing(t_redir *r, char **path, int right)
{
	int32_t		exist;
	int32_t		r_or_w;

	r_or_w = (right & O_WRONLY) ? W_OK : R_OK;
	printf("r_or_w = [%d]\n", r_or_w);
	printf("right2 = [%d]\n", right);
	*path = (*r->file == '/') ? ft_strdup(r->file) : create_abs_path(r->file);
	if (((exist = check_access(*path, r_or_w)) & (E_ACCES | E_NOENT | E_ISDIR)))
		return (exist);
	if (r->type == LESS && access(*path, F_OK)) //boff boff
		return (2);
	return (SUCCESS);
}

//						target>&source 

static uint8_t		error_handling(t_process *p, t_redir *r, int error)
{

	p->ret = 1;
	p->status = FAILED;
	ft_dprintf(p->std[2], "-->error redir<--   type = [%d]   error = [%d]\n", r->type, error);
	ft_printf("-->error redir<--   type = [%d]   error = [%d]\n", r->type, error);
	return (FAILURE);
}


int		bad_fd(int fd)
{
	struct stat buf;

	if (fstat(fd, &buf))
	{
		perror("fstat: ");
		return (FAILURE);
	}
	return (SUCCESS);
}

int		redir_gear(t_process *p, t_redir *r, uint32_t target, uint32_t right)
{
	int32_t		source;
	int32_t		error;
	char		*path;

	path = NULL;
	error = 1;
	if ((error = path_gearing(r, &path, right)))
		return (error_handling(p, r, error));
	if ((source = open(path, right, 0644)) == -1)
		perror("la open");
	if ((dup2(source, target) == -1))
		perror("dup2");
	if ((close(source) == -1))
		perror("close");
	printf("source = [%d]\ntarget = [%d]\n", source, target);
	return (SUCCESS);
}

int		redir_file(t_process *p, t_redir *r)
{
	uint32_t	target;
	uint32_t	right;
	uint8_t		io;

	io = (r->type == GREAT || r->type == DGREAT) ? 1 : 0;
	right = (io == 1) ? (O_WRONLY | O_CREAT) : O_RDONLY;
	target = (*r->io_num) ? ft_atoi(r->io_num) : io;
	printf("right 1 = [%d]\n", right);
	if (target > 255)
		return (error_handling(p, r, 1));
	if (r->type == GREAT)
		right |= O_TRUNC;
	else if (r->type == DGREAT)
		right |= O_APPEND;
	return (redir_gear(p, r, target, right));
}

int		redir_fd(t_process *p, t_redir *r)
{
	uint8_t		io;
	uint32_t	target;
	int32_t		source;

	io = (r->type == LESSAND) ? 0 : 1;
	target = (*r->io_num) ? ft_atoi(r->io_num) : io;
	source = (*r->file == '-') ? -1 : ft_atoi(r->file);
	if (target > 255)
		return (error_handling(p, r, 1));
	printf("source = %d\n", source);
	printf("target = %d\n", target);
	if (source == -1)
	{
		close(target);
		return (SUCCESS);
	}
	if (bad_fd(source))
		return (error_handling(p, r, 1));
	if (dup2(source, target) == -1)
		perror("dup2 redir fd:");
	return (SUCCESS);
}


int		redir_gearing(t_process *p, t_redir *r)
{
	if (r->type != LESSAND && r->type != GREATAND)
		return (redir_file(p, r));
	else
		return (redir_fd(p, r));
}

int		process_redir(t_process *p, t_list *redir)
{
	uint8_t	set;

	while (redir)
	{
		set = redir_gearing(p, redir->data);
		printf("set = %d\n", set);
		if (set)
		{
			p->status = FAILED;
			return (FAILURE);
		}
		redir = redir->next;
	}
	return (SUCCESS);
}





