#include "parser.h"
#include "exec.h"
#include "libft.h"
#include "var.h"
#include "ft_printf.h"
#include "sh.h"
#include <unistd.h>

#include <termios.h>

#include <stdio.h> //perror !!!!]]
#include <stdlib.h>


#include "sh.h"

void	set_signal_child(void)
{
	signal(SIGHUP, SIG_DFL);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	signal(SIGPIPE, SIG_DFL);
	signal(SIGCHLD, SIG_DFL);
	signal(SIGTSTP, SIG_DFL);
	signal(SIGCONT, SIG_DFL);
	signal(SIGTTIN, SIG_DFL);
	signal(SIGTTOU, SIG_DFL);
}

/*    plus besoin si strjoin valide avec stdarg
 *char		*ft_create_var_env(char **ctab, int len0, int len1)
 *{
 *    char	*dst;
 *
 *    if (!(dst = ft_strnew(len0 + len1 + 1)))
 *        exit(EXIT_FAILURE);
 *    ft_strcat(dst, ctab[0]);
 *    ft_strcat(dst, "=");
 *    ft_strcat(dst, ctab[1]);
 *    return (dst);
 *}
 */


/*					    ^        										*/
/*					    |        										*/
/********************TOOOOLLLLLLLLLLLLLLLLS*******************************/






static uint8_t		ft_echo(t_job *j, t_process *p)
{
		printf("ECHO builtin manquant\n");
		(void)j;
		(void)p;
		return (0);
}

static uint8_t		ft_cd(t_job *j, t_process *p)
{
		printf("CD builtin manquant\n");
		(void)j;
		(void)p;
		return (0);
}

static uint8_t		ft_hash(t_job *j, t_process *p)
{
		printf("HASH builtin manquant\n");
		(void)j;
		(void)p;
		return (0);
}


uint8_t		builtin_process(t_job *j, t_process *p)
{
	uint8_t		(*tab_f[7])(t_job *, t_process *);

	tab_f[0] = ft_echo;
	tab_f[1] = ft_exit;
	tab_f[2] = ft_cd;
	tab_f[3] = ft_env;
	tab_f[4] = ft_setenv;
	tab_f[5] = ft_unsetenv;
	tab_f[6] = ft_hash;
	if ((p->ret = tab_f[(p->setup >> 14)](j, p)))
		p->status = FAILED;
	else
		p->status = COMPLETED;
	return (p->ret);
}


int		parent_process(t_job *job, t_process *process, int fd_pipe, char **envp)
{
	if (fd_pipe)
		if (close(fd_pipe) == -1)
			perror("[Parent process] close error:");

	if (cfg_shell()->interactive) //singelton obliger?
	{
		if (job->pgid == 0)
			job->pgid = process->pid;
		setpgid(process->pid, job->pgid);
	}
	if (job->fg)// pour tous les process ?
		if (tcsetpgrp(STDIN_FILENO, job->pgid))
			perror("[PARENT PROCESS] error tcsetpgrp");
	ft_del_tab((void **)envp);
	return (0);
}

int		child_process(t_job *job, t_process *p, int fd_pipe, char **envp)
{
	if (fd_pipe)
		if (close(fd_pipe) == -1)
			perror("[child process] close error:");
	p->pid = getpid();
	if (cfg_shell()->interactive) //singelton obliger?
	{
		if (job->pgid == 0)
			job->pgid = p->pid;
		setpgid(p->pid, job->pgid);
		set_signal_child();
	}
	if (job->fg)
		if (tcsetpgrp(STDIN_FILENO, job->pgid) == -1)
			perror("[CHILD PROCESS] error tcsetpgrp");
	do_pipe(p);
	process_redir(p, p->redir);
	process_errors_handling(p);
	if (p->setup & BUILTIN)
		exit(builtin_process(job, p));  //que faire de envp??????
	if ((execve(p->path, p->av, envp)) == -1)
		ft_ex("[Fatal Error] EXECVE\nexit\n");
	exit(1);
}

int		fork_process(t_job *job, t_process *p)
{
	char	**envp;

	envp = create_tab_var(p->env, 0); //problematique, a voir ac l'assignement
	p->status = RUNNING;
	if ((p->pid = fork()) == -1)
		perror("fork:");
	if (!(p->pid))
		return (child_process(job, p, job->pipe.fd[0], envp));
	if (p->pid)
		return (parent_process(job, p, job->pipe.fd[1], envp));
	return (0);
}

void	run_process(t_job *j, t_process *p)
{
	process_type(p);
	if (p->setup & BUILTIN && !(p->setup & PIPE_ON))
	{
		process_redir(p, p->redir);
		builtin_process(j, p);
		do_my_dup2(j->std[0], STDIN_FILENO);
		do_my_dup2(j->std[1], STDOUT_FILENO);
		do_my_dup2(j->std[2], STDERR_FILENO);
	}
	else
		fork_process(j, p);
	return ;
}

uint8_t		routine_ending_job(t_cfg *shell, t_job *job)
{
	char	*ret;

	if (!shell->interactive)
	{
		wait_process(job);
	}
	else if (job->fg)
	{
	/*	tcsetpgrp(STDIN_FILENO, job->pgid) // seulememt ici du coup ?? */
		wait_process(job);
		tcsetpgrp(STDIN_FILENO, shell->pid);
		set_termios(TCSADRAIN, &shell->term_origin);
	}
	else		//bg gerer son return pour cas particuler ? doit avoir un wait ?
		return (0);
	ret = ft_itoa(job->ret);
	ft_setvar(&shell->intern, "$", ret);
	ft_strdel(&ret);
	return (job->ret);
}

uint8_t		run_job(t_cfg *shell, t_job *job, t_list *process)
{
	while (process)
	{
		routine_process(shell, process, &job->pipe);
		run_process(job, process->data);
		process = process->next;
		if (job->pipe.tmp)
			if (close(job->pipe.tmp) == -1)
				perror("[check and do pipe] close error:");  //perror
	}
	return (routine_ending_job(shell, job));
}
