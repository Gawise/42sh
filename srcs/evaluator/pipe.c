#include "exec.h"
#include "libft.h"
#include "sh.h"
#include "var.h"
#include <unistd.h>

int		do_pipe(t_process *p)
{
	/* Apply all redirection's child
	 * shall be call after set std[3] with all process : pipe > < 
	 */
	if (p->std[0] != STDIN_FILENO &&
			p->std[0] != -1)
	{
		printf("rentrei cmd %s \n", p->cmd);
		if (dup2(p->std[0], STDIN_FILENO) == -1)
			perror("[do_pipe]1 dup2 error:");
		if (close(p->std[0]) == -1)
			perror("[do_pipe]1 close error:");
	}
	if (p->std[1] != STDOUT_FILENO &&
			p->std[0] != -1)
	{
		printf("rentrei cmd %s \n", p->cmd);
		if (dup2(p->std[1], STDOUT_FILENO) == -1)
			perror("[do_pipe]2 dup2 error:");
		if (close(p->std[1]) == -1)
			perror("[do_pipe]2 close error:");
	}
	return (SUCCESS);
}

int		routine_process(t_cfg *shell, t_list *process, t_pipe *fd)
{
	/*  Carefull : |&   not supported  */
	/*  Need to check somewhere MAX_FD */
	t_process	*manage;
	t_list		*env;

	env = shell->env;
	manage = process->data;
	manage->env = ft_lstdup(env, env->size, cpy_var_list);
	fd->tmp = fd->fd[0];
	fd->fd[0] = 0;				/* set a 0, voir si pas mieux a -1*/
	fd->fd[1] = 0;
	if (!process->next)
		return (0);
	if (pipe(fd->fd) == -1)
		perror("pipe:");
	manage->std[1] = fd->fd[1];
	manage->setup += PIPE_ON;		/*Useless now    */
	manage = process->next->data;
	manage->std[0] = fd->fd[0];
	manage->setup += PIPE_ON;  /*Useless now    */
	return (0);
}

