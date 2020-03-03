#include "exec.h"
#include "libft.h"
#include "struct.h"





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


int		remove_file_name(char *s)
{
	int		i;
	char	tmp;
	int		ret;

	printf("rentre dans remove\n");
	i = ft_strrchri(s, '/');

	printf("s = [%s]\n", s);
	tmp = s[i + 1];
	s[i + 1] = '\0';
	printf("s = [%s]\n", s);
	ret = path_errors(s, 0);
	s[i + 1] = tmp;
	return (ret);
}


int		check_path(char *s)
{
	int		ret;
	char	*tmp;

	tmp = NULL;
	if (*s == '/')
		ret = path_errors(s, 1);
	else
	{
		tmp = create_abs_path(s);
		ret = path_errors(tmp, 1);
		printf("tmp = [%s]\n", tmp);
		ft_strdel(&tmp);
	}
	return (ret);
}

int		create_file(t_redir *r)
{
	int ret;
	ret = check_path(r->file);
	if (ret & E_NOENT)
		ret = remove_file_name(r->file);
	return (ret);
}

int		process_redir(t_process *p, t_list *redir)
{
	(void)p;
	while (redir)
	{
		printf("la [%d]\n", create_file(redir->data));
		redir = redir->next;
	}
	return (0);
}
