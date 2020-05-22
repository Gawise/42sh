#include "ft_printf.h"
#include "struct.h"
#include "sh.h"

void	debug_print_all(t_job *j, t_list *process, char *where)
{
	t_cfg		*shell;
	t_process	*p;

	shell = cfg_shell();
	if (!shell->debug)
		return ;
	ft_dprintf(shell->debug, "\n\n-----------\t [DEBUG] Print all\t %s\n", where);
	ft_dprintf(shell->debug, "\t---- [PROCESS]\n");

	while (process)
	{
		p = process->data;
		ft_dprintf(shell->debug, "CMD = [%s]\tPATH = [%s]\t PID = [%d]\n", p->cmd, p->path, p->pid);
		ft_dprintf(shell->debug, "SETUP = [%032b]\n", p->setup);
		ft_dprintf(shell->debug, "STATUS = [%d]\t RET = [%d]\n", p->status, p->ret);
		process = process->next;
	}
	ft_dprintf(shell->debug, "\t---- [JOB]\n");
	ft_dprintf(shell->debug, "CMD = [%s]\tID = [%d]\tFG = [%d]\n", j->cmd, j->id, j->fg);
	ft_dprintf(shell->debug, "STATUS = [%d]\tRET = [%d]\n", j->status, j->ret);
	ft_dprintf(shell->debug, "-----------\n", where);
}

void	debug_print_process(t_job *j, t_process *p, char *where)
{
	t_cfg		*shell;

	shell = cfg_shell();
	if (!shell->debug)
		return ;
	ft_dprintf(shell->debug, "\n\n-----------\t [DEBUG] Print process\t %s\n", where);
	ft_dprintf(shell->debug, "\t---- [JOB]\n");
	ft_dprintf(shell->debug, "CMD = [%s]\tID = [%d]\tFG = [%d]\n", j->cmd, j->id, j->fg);
	ft_dprintf(shell->debug, "\t---- [PROCESS]\n");
	ft_dprintf(shell->debug, "CMD = [%s]\tPATH = [%s]\tPID = [%d]\n", p->cmd, p->path, p->pid);
	ft_dprintf(shell->debug, "SETUP = [%032b]\n", p->setup);
	ft_dprintf(shell->debug, "STATUS = [%d]\n", p->status);
	ft_dprintf(shell->debug, "\t---- \n");
}

void	debug_print_lstfd(t_list *fd)
{
	int16_t	*t;
	t_cfg	*shell;

	shell = cfg_shell();
	if (!shell->debug)
		return ;
	while(fd)
	{
		t = fd->data;
		ft_dprintf(shell->debug, "fd[0] = [%d]\tfd[1] = [%d]\n", t[0], t[1]);
		fd = fd->next;
	}

}

void	debug_print_allfdjob(t_list *process)
{
	t_process	*p;
	t_cfg		*shell;

	shell = cfg_shell();
	p = process->data;
	if (!shell->debug)
		return ;
	if (!p->fd)
	{
		ft_dprintf(shell->debug, "No redirection in all process job\n");
		return ;
	}
	ft_dprintf(shell->debug, "Print all fd\n");
	while (process)
	{
		p = process->data;
		ft_dprintf(shell->debug, "Process [%s]\n", p->cmd);
		debug_print_lstfd(p->fd);
		process = process->next;
	}
}
