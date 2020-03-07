#include "exec.h"
#include "libft.h"
#include "sh.h"
#include <unistd.h>

int		do_dup(t_process *p)
{
	/* Apply all redirection's child
	 * shall be call after set std[3] with all process : pipe > < 
	*/

	//implique de jamais fermer 0 et 1 si non risque de new fd avec des nb;
	if (p->std[0] != STDIN_FILENO)
	{
		if (dup2(p->std[0], STDIN_FILENO) == -1)
			ex("[do_pipe] dup2 error:");
		if (close(p->std[0]) == -1)
			ex("[do_pipe] close error:");
	}
	if (p->std[1] != STDOUT_FILENO)
	{
		if (dup2(p->std[1], STDOUT_FILENO) == -1)
			ex("[do_pipe] dup2 error:");
		if (close(p->std[1]) == -1)
			ex("[do_pipe] close error:");
	}
	if (p->std[2] != STDERR_FILENO)
	{
		if (dup2(p->std[2], STDERR_FILENO) == -1)
			ex("[do_pipe] dup2 error:");
		if (close(p->std[2]) == -1)
			ex("[do_pipe] close error:");
	}
	return (SUCCESS);
}

int		routine_set_pipe(t_list *process, t_pipe *fd)
{
	/*  Carefull : |&   not supported  */
	/*  Need to check somewhere MAX_FD */
	t_process	*manage;

	fd->tmp = fd->fd[0];
	fd->fd[0] = 0;				/* set a 0, voir si pas mieux a -1*/
	fd->fd[1] = 0;
	if (!process->next)
		return (0);
	if (pipe(fd->fd) == -1)
		ex("pipe:");
	manage = process->data;
	manage->std[1] = fd->fd[1];
	manage->setup += LEFT;		/*Useless now    */
	manage = process->next->data;
	manage->std[0] = fd->fd[0];
	manage->setup += RIGHT;  /*Useless now    */
	return (0);
}

