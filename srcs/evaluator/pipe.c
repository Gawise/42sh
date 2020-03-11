#include "exec.h"
#include "libft.h"
#include "sh.h"
#include <unistd.h>

int		do_pipe(t_process *p)
{
	/* Apply all redirection's child
	 * shall be call after set std[3] with all process : pipe > < 
	 */
	if (p->std[0] != STDIN_FILENO)
	{
		if (dup2(p->std[0], STDIN_FILENO) == -1)
			perror("[do_pipe]1 dup2 error:");
		if (close(p->std[0]) == -1)
			perror("[do_pipe]1 close error:");
	}
	if (p->std[1] != STDOUT_FILENO)
	{
		if (dup2(p->std[1], STDOUT_FILENO) == -1)
			ex("[do_pipe]2 dup2 error:");
		if (close(p->std[1]) == -1)
			perror("[do_pipe]2 close error:");
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

