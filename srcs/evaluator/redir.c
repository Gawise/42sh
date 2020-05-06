#include "exec.h"
#include "libft.h"
#include "struct.h"
#include "ft_printf.h"
#include "var.h"
#include "sh.h"
#include <fcntl.h>

uint8_t		redir_gear(t_process *p, t_redir *r, uint32_t target, uint32_t right)
{
	int32_t		source;
	int32_t		error;
	char		*path;

	path = NULL;
	if ((error = path_gearing(r, &path, right)))
		return (redir_errors_handling(p, error, path, 0));
	if ((source = open(path, right, 0644)) == -1)
		perror("la open");  //perror
	do_my_dup2(source, target);
	if ((close(source) == -1))
		perror("close");  //perror
	ft_strdel(&path);
	return(SUCCESS);
}

uint8_t		redir_file(t_process *p, t_redir *r)
{
	uint32_t	target;
	uint32_t	right;
	uint8_t		io;

	io = (r->type == GREAT || r->type == DGREAT) ? 1 : 0;
	right = (io == 1) ? (O_WRONLY | O_CREAT) : O_RDONLY;
	target = (*r->io_num) ? ft_atoi(r->io_num) : io;
	if (target > 255)
		return(redir_errors_handling(p, 0, 0, target));
	if (r->type == GREAT)
		right |= O_TRUNC;
	else if (r->type == DGREAT)
		right |= O_APPEND;
	return(redir_gear(p, r, target, right));
}

uint8_t		redir_fd(t_process *p, t_redir *r)
{
	uint8_t		io;
	uint32_t	target;
	int32_t		source;

	io = (r->type == LESSAND) ? 0 : 1;
	target = (*r->io_num) ? ft_atoi(r->io_num) : io;
	source = (*r->file == '-') ? -1 : ft_atoi(r->file);
	if (target > 255)
		return (redir_errors_handling(p, 0, 0, target));
	if (source == -1)
	{
		close(target);
		return (SUCCESS);
	}
	if (bad_fd(source))
		return (redir_errors_handling(p, 0, 0, source));
	do_my_dup2(source, target);
	return (SUCCESS);
}

uint8_t		redir_heredoc(t_process *p, t_redir *r)
{
	int32_t		fd;
	char	*path;

	path = NULL;
	ft_asprintf(&path, "/tmp/21-42sh-heredoc-%p", &fd);
	if (((fd = open(path, O_CREAT | O_WRONLY, 0644)) == -1))
		perror("redir_heredoc open failled"); //perror
	if (write(fd, r->file, ft_strlen(r->file)) == -1)
		perror("write redir_heredoc"); //perror
	close(fd);
	ft_strdel(&r->file);
	r->file = path;
	return (redir_file(p, r));
}

void		process_redir(t_process *p, t_list *redir)
{
	uint8_t		error;
	t_redir		*r;

	while (redir)
	{
		r = redir->data;
		if (r->type == DLESS || r->type == DLESSDASH)
			error = redir_heredoc(p, r);
		else if (r->type != LESSAND && r->type != GREATAND)
			error = redir_file(p, r);
		else
			error = redir_fd(p, r);
		if (error)
			exit(1);
		redir = redir->next;
	}
}
