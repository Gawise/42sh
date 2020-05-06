#include "ft_printf.h"
#include "struct.h"
#include "sh.h"

void	debug_print_all(t_job *j, t_list *process, char *where)
{
	t_cfg		*shell;
	t_process	*p;

	shell = cfg_shell();
	ft_dprintf(shell->debug, "\n\n-----------\t [DEBUG] Print all\t %s\n", where);
	ft_dprintf(shell->debug, "\t---- [PROCESS]\n");

	while (process)
	{
		p = process->data;
		ft_dprintf(shell->debug, "CMD = [%s]\tPATH = [%s]\t PID = [%d]\n", p->cmd, p->path, p->pid);
		ft_dprintf(shell->debug, "SETUP = [%032b]\n", p->setup);
		ft_dprintf(shell->debug, "STATUS = [%d]\n", p->status);
		process = process->next;
	}
	ft_dprintf(shell->debug, "\t---- [JOB]\n");
	ft_dprintf(shell->debug, "CMD = [%s]\tID = [%d]\tFG = [%d]\n", j->cmd, j->id, j->fg);
	ft_dprintf(shell->debug, "STATUS = [%d]\tRET = [%d]\n", j->status, j->ret);
	ft_dprintf(shell->debug, "-----------\n", where);
}
