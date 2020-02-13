#include "libft.h"
#include "exec.h"
#include "parser.h"


#include <stdio.h>   //debug

int		lvl_simple_cmd(t_cfg *shell, t_list *s_cmd, uint8_t fg)
{
	t_job job;

	cmd_to_job(&job, s_cmd);
	job.fg = fg;

	job.var = env_list_cpy(shell->var); //refaire cette fonction en triant var

	run_job(shell, &job, job.process);
	routine_clean_job(&job);
	return (0);
}

int		lvl_and_or(t_cfg *shell, t_list *lst)
{
	t_and_or *andor;

	andor = lst->data;
	lvl_simple_cmd(shell, andor->s_cmd, !andor->background);
	if (lst->next && condition_respectee())
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

	printf("\n\n----------- eval -----------\n\n");
	lvl_cmd_table(shell, cmd_table);
	printf("----------- eval -----------\n\n");
	return (0);
}
