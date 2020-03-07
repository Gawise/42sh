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

int		path_engine(t_redir *r, char **path, int right)
{
	int		exist;

	*path = (*r->file == '/') ? ft_strdup(r->file) : create_abs_path(r->file);
	if (((exist = check_access(*path, right)) & (E_ACCES | E_NOENT)))
		return (exist);
	return (SUCCESS);
}

//						target>&source 

static uint8_t		error_handling(t_process *p, t_redir *r, int error)
{

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

int		do_change(int *target, int source)
{
	if (*target > 2)   // regle builtin ??
		if (close(*target) == -1)
			perror("close: ");
	*target = source;
	return (SUCCESS);
}

int		redir_great(t_process *p, t_redir *r)
{
	int		target;
	int		source;
	int		error;
	char	*path;

	path = NULL;
	error = 1;
	target = (*r->io_num) ? ft_atoi(r->io_num) : 1;
	printf("target = [%d]\n", target);
	if ((target > 255) || (error = path_engine(r, &path, W_OK)))
		return (error_handling(p, r, error));

	printf("path = [%s]\n", path);
	if ((source = open(path, O_CREAT | O_TRUNC | O_RDWR, 0644)) == -1)
		perror("la open");
	if (target > 2)
	{
		if ((dup2(source, target) == -1))
			perror("dup2");
		if ((close(source) == -1))
			perror("close");
	}
	else
	{
		p->std[target] = source;
		ft_strdel(&r->io_num);
		r->io_num = ft_itoa(source);
	}
	printf("source = [%d]\ntarget = [%d]\n", source, target);
	return (SUCCESS);
}

int		find_ft_redir(t_process *p, t_redir *r)
{
	int	 (*tabft[7])(t_process *, t_redir *);

	tabft[2] = &redir_great;

	printf("type = [%d]\n", r->type);
	return (tabft[r->type - 10](p, r));

}

int		process_redir(t_process *p, t_list *redir)
{
	uint8_t	set;

	while (redir)
	{
		set = find_ft_redir(p, redir->data);
		printf("set = %d\n", set);
		if (set)
		{
			p->status = FAILED;
			return (FAILURE);
		}
		redir = redir->next;
	}
	return (0);
}





/*
 *int		init_redir(t_process *p, t_redir *r, uint8_t *set)
 *{
 *    char	*path;
 *    int		err;
 *
 *    path = NULL;
 *    if ((*set = setting(r)) & 1)
 *        err = (*set & 2 ) ? path_engine(r, path, W_OK) : path_engine(r, path, R_OK);
 *    else
 *        err = fd_engine(p, r, set);
 *    ft_strdel(&path);
 *    if (err)
 *        return (error_handling(p, r, err)); //failure
 *}
 *int		setting(t_redir *r)
 *{
 *    uint8_t i;
 *
 *    i = 0;
 *    if (r->type != LEESAND && r_type != GREATAND)  //fd or file
 *        i = 1;	//file
 *    if (r->type == GREAT || r->type == DGREAT || r->type == GREATAND)  //input or output
 *        i += 2; //input
 *    return (i);
 *}
 */
