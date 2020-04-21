#include "exec.h"
#include "sh.h"
#include "var.h"
#include "ft_printf.h"

uint8_t		process_errors_handling(t_process *p)
{
	char	*namesh;

	if (!(namesh = find_var_value(cfg_shell()->intern, "PS1")))
		namesh = NAME_SH;
	if (!(p->setup & ERROR))
		return (SUCCESS);
	if (!p->cmd)
		exit(EXIT_SUCCESS);
	p->setup &= ~ERROR;
	if (p->setup & E_UNFOUND)
		ft_dprintf(2, "%s: %s: command not found\n", namesh, p->cmd);
	else if (p->setup & E_ISDIR)
		ft_dprintf(2, "%s: %s: is a directory\n", namesh, p->path );
	else if (p->setup & E_NOENT)
		ft_dprintf(2, "%s: %s: No such file or directory\n", namesh, p->path);
	else if (p->setup & E_ACCES)
		ft_dprintf(2, "%s: %s: Permission denied\n", namesh, p->path);
	else if (p->setup & E_LOOP)
		ft_dprintf(2, "%s: %s: Too many links\n", namesh, p->path);
	else if (p->setup & E_NTL)
		ft_dprintf(2, "%s: %s: File name too long\n", namesh, p->path);
	p->ret = p->setup & (E_UNFOUND | E_NOENT) ? 127 : 126;
	exit(p->ret);
}

uint8_t		redir_errors_handling(t_process *p, uint32_t error, char *info, int32_t fd)
{
	char 	*namesh;

	error &= ~ERROR;
	namesh = find_var_value((cfg_shell())->intern, "PS1");
	if (!namesh)
		namesh = NAME_SH;
	p->ret = 1;
	p->status |= FAILED;
	if (fd)
		ft_dprintf(STDERR_FILENO, "%s: %d: Bad file descriptor\n", namesh, fd);
	else if (error & E_ISDIR)
		ft_dprintf(STDERR_FILENO, "%s: %s: is a directory\n", namesh, info);
	else if (error & E_NOENT)
		ft_dprintf(STDERR_FILENO, "%s: %s: No such file or directory\n", namesh, info);
	else if (error & E_ACCES)
		ft_dprintf(STDERR_FILENO, "%s: %s: Permission denied\n", namesh, info);
	ft_strdel(&info);
	return (FAILURE);
}
