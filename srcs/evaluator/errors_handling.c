#include "exec.h"
#include "sh.h"
#include "var.h"
#include "ft_printf.h"

uint32_t		process_errors_handling(t_process *p, uint32_t err)
{
	if (!p->cmd && (p->assign || p->redir))
		return (SUCCESS);
	p->status |= FAILED;
	if (err & E_UNFOUND)
		ft_asprintf(&p->message, "%s: %s: %s\n", PROJECT, p->cmd, STR_UNFOUND);
	else if (err & E_ISDIR)
		ft_asprintf(&p->message, "%s: %s: %s\n", PROJECT, p->path, STR_ISDIR);
	else if (err & E_NOENT)
		ft_asprintf(&p->message, "%s: %s: %s\n", PROJECT, p->path, STR_NOENT);
	else if (err & E_ACCES)
		ft_asprintf(&p->message, "%s: %s: %s\n", PROJECT, p->path, STR_ACCES);
	else if (err & E_LOOP)
		ft_asprintf(&p->message, "%s: %s: %s\n", PROEJECT, p->path, STR_LOOP);
	else if (err & E_NTL)
		ft_asprintf(&p->message, "%s: %s: %s\n", PROJECT, p->path, STR_NTL);
	p->ret = err & (E_UNFOUND | E_NOENT) ? 127 : 126;
	return (P_ERROR | err);
}

uint32_t		redir_errors_handling(t_process *p, uint32_t error, char *info, int32_t fd)
{
	p->status |= FAILED;
	if (error & E_BADFD)
		ft_asprintf(&p->message, "%s: %d: Bad file descriptor\n", PROJECT, fd);
	else if (error & E_ISDIR)
		ft_asprintf(&p->message, "%s: %s: %s\n", PROJECT, info, STR_ISDIR);
	else if (error & E_NOENT)
		ft_asprintf(&p->message, "%s: %s: %s\n", PROJECT, info, STR_NOENT);
	else if (error & E_ACCES)
		ft_asprintf(&p->message, "%s: %s: %s\n", PROJECT, info, STR_ACCES);
	ft_strdel(&info);
	p->ret = 1;
	return (error | R_ERROR);
}
