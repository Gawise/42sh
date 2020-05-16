#include "exec.h"
#include "sh.h"
#include "var.h"
#include "ft_printf.h"

uint8_t		process_errors_handling(t_process *p)
{
	if (!(p->setup & ERROR))
		return (SUCCESS);
	if (!p->cmd && (p->assign || p->redir))
		exit(EXIT_SUCCESS);
	p->setup &= ~ERROR;
	if (p->setup & E_UNFOUND)
		ft_dprintf(2, "%s: %s: %s\n", STR_UNFOUND, PROJECT, p->cmd);
	else if (p->setup & E_ISDIR)
		ft_dprintf(2, "%s: %s: %s\n", STR_ISDIR, PROJECT, p->path );
	else if (p->setup & E_NOENT)
		ft_dprintf(2, "%s: %s: %s\n", STR_NOENT, PROJECT, p->path);
	else if (p->setup & E_ACCES)
		ft_dprintf(2, "%s: %s: %s\n", STR_ACCES PROJECT, p->path);
	else if (p->setup & E_LOOP)
		ft_dprintf(2, "%s: %s: %s\n", STR_LOOP PROJECT, p->path);
	else if (p->setup & E_NTL)
		ft_dprintf(2, "%s: %s: %s\n", STR_NTL, PROJECT, p->path);
	p->ret = p->setup & (E_UNFOUND | E_NOENT) ? 127 : 126;
	exit(p->ret);
}

uint8_t		redir_errors_handling(t_process *p, uint32_t error, char *info, int32_t fd)
{
	error &= ~ERROR;
	p->ret = 1;
	p->status = FAILED;
	if (fd)
		ft_dprintf(STDERR_FILENO, "%s: %d: Bad file descriptor\n", PROJECT, fd);
	else if (error & E_ISDIR)
		ft_dprintf(STDERR_FILENO, "%s: %s: %s\n", STR_ISDIR, PROJECT, info);
	else if (error & E_NOENT)
		ft_dprintf(STDERR_FILENO, "%s: %s: %s\n", STR_NOENT, PROJECT, info);
	else if (error & E_ACCES)
		ft_dprintf(STDERR_FILENO, "%s: %s: %s\n", STR_ACCES, PROJECT, info);
	ft_strdel(&info);
	return (FAILURE);
}
