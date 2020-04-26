#include "exec.h"
#include "libft.h"
#include "sh.h"
#include "var.h"
#include <unistd.h>

static void		assign_for_unfound(t_cfg *shell, t_list *assignment)
{
	t_list			*tmp;
	t_assignment	*assign;

	while (assignment)
	{
		assign = assignment->data;
		if ((tmp = find_var(shell->env, assign->var)))
			ft_setvar(&tmp, assign->var, assign->val);
		else
			ft_setvar(&shell->intern, assign->var, assign->val);
		assignment = assignment->next;
	}
}

static void		assign_for_b_special(t_cfg *shell, t_process *p, t_list *assignment)
{
	t_assignment	*assign;

	while (assignment)
	{
		assign = assignment->data;
		ft_setvar(&p->env, assign->var, assign->var);
		ft_setvar(&shell->env, assign->var, assign->var);
		assignment = assignment->next;
	}
}

void			process_assign(t_cfg *shell, t_process *p, t_list *assignment)
{
	t_assignment	*assign;

	if (!assignment)
		return ;
	if (p->setup & E_UNFOUND)
		assign_for_unfound(shell, assignment);
	else if (p->setup & B_SPECIAL)
		assign_for_b_special(shell, p, assignment);
	else
	{
		while (assignment)
		{
			assign = assignment->data;
			ft_setvar(&p->env, assign->var, assign->var);
			assignment = assignment->next;
		}
	}
}

void			do_pipe(t_process *p)
{
	if (p->std[0] != STDIN_FILENO &&
			p->std[0] != -1)
	{
		if (dup2(p->std[0], STDIN_FILENO) == -1)
			perror("[do_pipe]1 dup2 error:");
		if (close(p->std[0]) == -1)
			perror("[do_pipe]1 close error:");
	}
	if (p->std[1] != STDOUT_FILENO &&
			p->std[0] != -1)
	{
		if (dup2(p->std[1], STDOUT_FILENO) == -1)
			perror("[do_pipe]2 dup2 error:");
		if (close(p->std[1]) == -1)
			perror("[do_pipe]2 close error:");
	}
}

void		routine_process(t_cfg *shell, t_list *process, t_pipe *fd)
{
	t_process	*manage;
	t_list		*env;

	env = shell->env;
	manage = process->data;
	if (env)
		manage->env = ft_lstdup(env, env->size, cpy_var_list);
	fd->tmp = fd->fd[0];
	fd->fd[0] = 0;				/* set a 0, voir si pas mieux a -1*/
	fd->fd[1] = 0;
	if (!process->next)
		return ;
	if (pipe(fd->fd) == -1)
		perror("pipe:");      ///////////// perror 
	manage->std[1] = fd->fd[1];
	manage->setup += PIPE_ON;		/*Useless now    */
	manage = process->next->data;
	manage->std[0] = fd->fd[0];
	manage->setup += PIPE_ON;  /*Useless now    */
}