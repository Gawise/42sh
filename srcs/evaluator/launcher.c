#include "parser.h"
#include "exec.h"
#include "libft.h"
#include <unistd.h>
#include <stdio.h> //perror !!!!]]

#include "ft_printf.h"

#include <stdlib.h>
void	ex(char *s)
{
	perror(s);
	exit(EXIT_FAILURE);
}

#include <signal.h>
void	sig(int i)
{
	printf("sig detect = [%d]\n", i);
}

#include "sh.h"
void	signal_debug_printf(void)
{
	signal(SIGHUP, sig);
	signal(SIGINT, sig);
	signal(SIGQUIT, sig);
	signal(SIGPIPE, sig);
	signal(SIGCHLD, sig);
	signal(SIGTSTP, sig);
	signal(SIGTSTP, sig);
	signal(SIGCONT, sig);
	signal(SIGTTIN, sig);
	signal(SIGTTOU, sig);
}

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







int		builtin_process(void)
{

	return (0);
}


int		parent_process(t_job *job, t_process *process, int fd_pipe, char **envp)
{

	if (fd_pipe)
		if (close(fd_pipe) == -1)
			ex("[Parent process] close error:");
	if (job->pgid == 0)
		job->pgid = process->pid;
	process->status = RUNNING;
	setpgid(process->pid, job->pgid);
	if (job->fg)
		if (tcsetpgrp(STDIN_FILENO, job->pgid))
			perror("[PARENT PROCESS] error tcsetpgrp");
	ft_del_tab((void **)envp);
	return (0);
}

int		child_process(t_job *job, t_process *process, int fd_pipe, char **envp)
{
	//set signal:
	//https://www.gnu.org/software/libc/manual/html_node/Signal-Handling.html#Signal-Handling
	// Belek interractif or not

	if (fd_pipe)
		if (close(fd_pipe) == -1)
			ex("[child process] close error:");
	set_signal_child();
	process->pid = getpid();
	if (job->pgid == 0)
		job->pgid = process->pid;
	setpgid(process->pid, job->pgid); 		//not do if !fg ??
	 if (job->fg)
		if (tcsetpgrp(STDIN_FILENO, job->pgid) == -1)
			perror("[CHILD PROCESS] error tcsetpgrp");
	do_dup(process);
	if ((execve(process->path, process->av, envp)) == -1)
		ex("execve:");
	return (0);
}

int		fork_process(t_job *job, t_process *process)
{
	char	**envp;

	envp = create_tab_env(job->var, 0); //problematique, a voir ac l'assignement
	if ((process->pid = fork()) == -1)
		ex("fork:");
	if (!(process->pid))
		return (child_process(job, process, job->pipe.fd[0], envp));
	if (process->pid)
		return (parent_process(job, process, job->pipe.fd[1], envp));
	return (0);
}

int		run_process(t_cfg *shell, t_job *job, t_process *process)
{
	//faire les redir et open
	(void)shell;
	int debug = process_type(job->var, process); //return (127 si pas trouver)
	if (debug == 127) //pour eviter de lancer vide pour le moment
		return (1);
	return (fork_process(job, process));
}

int		run_job(t_cfg *shell, t_job *job, t_list *process)
{
	//set le terminal et sauv ?
	//https://www.gnu.org/software/libc/manual/html_node/Functions-for-Job-Control.html#Functions-for-Job-Control
	//https://www.gnu.org/software/libc/manual/html_node/Process-Completion.html#Process-Completion
	//https://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html#tag_18_09_01_01

	while (process)
	{
		routine_set_pipe(process, &job->pipe);
		run_process(shell, job, process->data);
		process = process->next;
		if (job->pipe.tmp)
			if (close(job->pipe.tmp) == -1)
				ex("[check and do pipe] close error:");
	}
	if (job->fg)
	{
		wait_process(job);
		if (tcsetpgrp(STDIN_FILENO, shell->pid) == -1)
			perror("[run job] error tcsetpgrp");
	}

	/*
	 *   if (!shell_is_interactive)
	 wait_for_job (j);
	 else if (foreground)
	 put_job_in_foreground (j, 0);
	 else
	 put_job_in_background (j, 0);
https://www.gnu.org/software/libc/manual/html_node/Stopped-and-Terminated-Jobs.html#Stopped-and-Terminated-Jobs
*/
	return (0);
}
