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
void	set_signal_child(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	signal(SIGTSTP, SIG_DFL);
	signal(SIGTTIN, SIG_DFL);
	signal(SIGTTOU, SIG_DFL);
	signal(SIGCHLD, SIG_DFL);
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

int		wait_job(t_cfg *shell, t_job *job, pid_t wait)
{
	pid_t pid_child;

	(void)shell;
	pid_child = waitpid(wait, &job->wstatus, WUNTRACED);// wait le dernier dunjob
	//pid_child = waitpid(-job->pgid, &job->wstatus, WUNTRACED);
	printf("sort du wait\n");
	return (0);
}

int		parent_process(t_job *job, t_process *process, int fd_pipe, char **envp)
{
	
	if (fd_pipe)
		if (close(fd_pipe) == -1)
			ex("[Parent process] close error:");
	if (job->pgid == 0)
		job->pgid = process->pid;
	
	/*setpgid(process->pid, job->pgid);			//wait job controler
	 *if (job->fg)
	 *    tcsetpgrp(STDIN_FILENO, job->pgid);  */

	ft_del_tab((void **)envp);
	return (process->pid);   //debug pour wait last processjob
}

int		child_process(t_job *job, t_process *process, int fd_pipe, char **envp)
{
	//set signal:
	//https://www.gnu.org/software/libc/manual/html_node/Signal-Handling.html#Signal-Handling
	// Belek interractif or not

	if (fd_pipe)
		if (close(fd_pipe) == -1)
			ex("[Parent process] close error:");
	process->pid = getpid();
	if (job->pgid == 0)
		job->pgid = process->pid;
	set_signal_child();
	do_dup(process);
	 /*setpgid(process->pid, job->pgid); 		//not do if !fg ??
	 *if (job->fg)
	 *    tcsetpgrp(STDIN_FILENO, job->pgid); */

	printf("lanchement d un process\n [%s]", process->cmd);
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

	//type_process ??
	//faire les redir et open
	(void)shell;
	int debug = process_type(job->var, process); //return (127 si pas trouver)
	if (debug == 127) //pour eviter de lancer vide pour le moment
		return (1);
	printf("process_type retour = [%d]\n", debug);
	return (fork_process(job, process));
}

int		run_job(t_cfg *shell, t_job *job, t_list *process)
{
	/* Add : Loop until we are in the foreground.  */
	//set le terminal et sauv ?
	//https://www.gnu.org/software/libc/manual/html_node/Functions-for-Job-Control.html#Functions-for-Job-Control
	//faire les pipes ?
	//envoyer les process
	//wait ici ?
	//https://www.gnu.org/software/libc/manual/html_node/Process-Completion.html#Process-Completion
	//exec_type ?
	//https://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html#tag_18_09_01_01

	pid_t wait; //debug

		while (process)
	{
		routine_set_pipe(process, &job->pipe);
		wait = run_process(shell, job, process->data);
		process = process->next;
		if (job->pipe.tmp)
			if (close(job->pipe.tmp) == -1)
				ex("[check and do pipe] close error:");
	}
	if (job->fg)
		wait_job(shell, job, wait);
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
