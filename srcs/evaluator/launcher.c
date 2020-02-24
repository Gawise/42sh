#include "parser.h"
#include "exec.h"
#include "libft.h"
#include <unistd.h>
#include <stdio.h> //perror !!!!]]


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
	signal(SIGTSTP, SIG_DFL);
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

/*
 *t_list		*create_pipe_list(int nb)
 *{
 *    t_list *new;
 *    t_rdir	elem;
 *
 *    new = NULL;
 *    elem.fd[0] = -1;
 *    elem.fd[1] = -1;
 *
 *    while (nb--)
 *        ft_lst_push_back(&new, &elem, sizeof(t_rdir))
 *            ;
 *    return (new);
 *}
 */






/*					    ^        										*/
/*					    |        										*/
/********************TOOOOLLLLLLLLLLLLLLLLS*******************************/






int		condition_respectee(void)
{
	return (0);
}

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

	printf("\t[PARANT PROCESS]  PID  = %d\t PGID = %d \n", process->pid, job->pgid);
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
	process->pid = getpid();
	if (job->pgid == 0)
		job->pgid = process->pid;
	set_signal_child();
	do_dup(process);
	setpgid(process->pid, job->pgid); 		//not do if !fg ??
	 if (job->fg)
		if (tcsetpgrp(STDIN_FILENO, job->pgid))
			perror("[CHILD PROCESS] error tcsetpgrp");

	printf("\t[CHILD PROCESS EXEC] \t[%s]\n\n\n", process->av[0]);
	if ((execve(process->path, process->av, envp)) == -1)
		ex("execve:");
	return (0);
}

int		fork_process(t_job *job, t_process *process)
{
	char	**envp;

	printf("\t[FORK PROCESS] [%s]\n", process->path);
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

	//type_process ??
	//faire les redir et open
	printf("\t[RUN PROCESS]\n");
	(void)shell;
	int debug = process_type(job->var, process); //return (127 si pas trouver)
	if (debug == 127) //pour eviter de lancer vide pour le moment
		return (1);
//	printf("process_type retour = [%d]\n", debug);
	return (fork_process(job, process));
}

int		run_job(t_cfg *shell, t_job *job, t_list *process)
{
	//set le terminal et sauv ?
	//https://www.gnu.org/software/libc/manual/html_node/Functions-for-Job-Control.html#Functions-for-Job-Control
	//faire les pipes ?
	//envoyer les process
	//wait ici ?
	//https://www.gnu.org/software/libc/manual/html_node/Process-Completion.html#Process-Completion
	//exec_type ?
	//https://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html#tag_18_09_01_01

	printf("\n\t[RUN JOB]  PID 21SH = [%d]\n", shell->pid);
	
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
		if (tcsetpgrp(STDIN_FILENO, shell->pid))
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
