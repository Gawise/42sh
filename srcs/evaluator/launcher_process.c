#include "parser.h"
#include "exec.h"
#include "libft.h"
#include "var.h"
#include "sh.h"
#include "ft_printf.h"
#include "job_control.h"
#include <unistd.h>

#include <termios.h>

#include <stdlib.h>

#include "sh.h"



static uint8_t		ft_fg(t_job *j, t_process *p)
{
		printf("FG builtin manquant\n");
		(void)j;
		(void)p;
		return (0);
}

static uint8_t		ft_bg(t_job *j, t_process *p)
{
		printf("BG builtin manquant\n");
		(void)j;
		(void)p;
		return (0);
}


uint8_t		builtin_process(t_job *j, t_process *p)
{
	uint8_t		(*tab_f[11])(t_job *, t_process *);

	tab_f[0] = ft_echo;
	tab_f[1] = ft_cd;
	tab_f[2] = ft_env;
	tab_f[3] = ft_setenv;
	tab_f[4] = ft_unsetenv;
	tab_f[5] = ft_hash;
	tab_f[6] = ft_exit;
	tab_f[7] = ft_jobs;
	tab_f[8] = ft_fg;
	tab_f[9] = ft_bg;
	tab_f[10] = ft_type;
	if (p->status == FAILED)
		return (p->ret);
	if ((p->ret = tab_f[(p->setup >> 14)](j, p)))
		p->status = FAILED;
	else
		p->status = COMPLETED;
	return (p->ret);
}

uint8_t		parent_process(t_job *job, t_process *process, int fd_pipe, char **envp)
{
	if (fd_pipe)
		if (close(fd_pipe) == -1)
			ft_ex("[Parent process] close error:"); ///debug
	//if (process->setup & ERROR)
	//	process->status = FAILED; // pour bg mais pourquoi ?
	if (cfg_shell()->interactive) //singelton obliger?
	{
		if (job->pgid == 0)
			job->pgid = process->pid;
		setpgid(process->pid, job->pgid);
		if (job->fg)// pour tous les process ?
			if (tcsetpgrp(STDIN_FILENO, job->pgid))
				ft_ex("[PARENT PROCESS] error tcsetpgrp"); //debug
	}
	ft_del_tab((void **)envp);
	return (SUCCESS);
}

uint8_t		child_process(t_job *job, t_process *p, int fd_pipe, char **envp)
{
	if (fd_pipe)
		if (close(fd_pipe) == -1)
			ft_ex("[child process] close error:"); //debug
	p->pid = getpid();
	if (cfg_shell()->interactive) //singelton obliger?
	{
		if (job->pgid == 0)
			job->pgid = p->pid;
		setpgid(p->pid, job->pgid);
		if (job->fg)
			if (tcsetpgrp(STDIN_FILENO, job->pgid) == -1)
				ft_ex("[CHILD PROCESS] error tcsetpgrp"); //debug
		set_signal_child();
	}
	do_pipe(p);
	do_redir(p->fd);
	if (p->setup & ERROR)
	{
		ft_dprintf(STDERR_FILENO,"%s", p->message);
		exit(p->ret);
	}
	if (p->setup & BUILTIN)
		exit(builtin_process(job, p));  //que faire de envp??????
	if ((execve(p->path, p->av, envp)) == -1)
		ft_ex(EXEXEC);
	exit(1);
}

uint8_t		fork_process(t_job *job, t_process *p)
{
	char	**envp;

	envp = create_tab_var(p->env, 0); //problematique, a voir ac l'assignement
	if ((p->pid = fork()) == -1)
		ft_ex(EX);
	if (!(p->pid))
		return (child_process(job, p, job->pipe.fd[0], envp));
	if (p->pid)
		return (parent_process(job, p, job->pipe.fd[1], envp));
	return (0);
}

void		run_process(t_cfg *shell, t_job *j, t_process *p)
{

	p->status = RUNNING | (p->status & ~WAITING);
	process_type(p);
	process_assign(shell, p, p->assign); // not cmd != false
	debug_print_process(j, p, "run_process");
	if (p->setup & BUILTIN && !(p->setup & PIPE_ON) && j->fg)
	{
		do_redir(p->fd);
		if (p->setup & R_ERROR)
			ft_dprintf(STDERR_FILENO,"%s", p->message);
		if ((p->setup & R_ERROR) &&
				(p->setup & B_SPECIAL) && !shell->interactive)
			exit(1);
		builtin_process(j, p);
		do_my_dup2(j->std[0], STDIN_FILENO);
		do_my_dup2(j->std[1], STDOUT_FILENO);
		do_my_dup2(j->std[2], STDERR_FILENO);
	}
	else
		fork_process(j, p);
	ft_lstiter(p->fd, close_fd);
	return ;
}
