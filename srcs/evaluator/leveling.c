#include "libft.h"
#include "exec.h"
#include "parser.h"
#include "sh.h"


#include <stdio.h>   //debug

int		condition_respectee(t_and_or *andor, int8_t lr)
{
	if (andor->type == AND_IF)
		return (lr == SUCCESS ? 1 : 0);
	if (andor->type == OR_IF)
		return (lr == SUCCESS ? 0 : 1);
	return (FAILURE);
}

int		lvl_simple_cmd(t_cfg *shell, t_list *s_cmd, uint8_t fg)
{
	uint8_t		tmp;
	t_job		job;

	cmd_to_job(&job, s_cmd);
	job.fg = fg;

	job.var = env_list_cpy(shell->var); //refaire cette fonction en triant var

	run_job(shell, &job, job.process);
	tmp = job.ret;				//en attendant var intern? 
	routine_clean_job(&job);
	return (tmp);
}

int		lvl_and_or(t_cfg *shell, t_list *lst)
{
	uint8_t		lr;				//en attendant var intern ?
	t_and_or	*andor;


	andor = lst->data;
	lr = lvl_simple_cmd(shell, andor->s_cmd, !andor->background);
	if (lst->next && condition_respectee(andor, lr))
		lvl_and_or(shell, lst->next);
	return (0);
}

int		lvl_cmd_table(t_cfg *shell, t_list *lst)
{
	t_cmd_table		*cmd;

	while (lst)
	{
		cmd = lst->data;
		lvl_and_or(shell, cmd->and_or);
		lst = lst->next;
	}
	return (0);
}

int		ft_eval(t_cfg *shell, t_list *cmd_table)
{

	printf("\n\n----------- eval -----------\n\n\n\n");
	set_signal_ign();
	signal(SIGCHLD, SIG_DFL);
	lvl_cmd_table(shell, cmd_table);
	printf("----------- eval -----------\n\n");
	return (0);
}
