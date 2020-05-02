#include "libft.h"
#include "ft_printf.h"
#include "exec.h"
#include "parser.h"
#include "sh.h"

uint8_t		condition_respected(t_and_or *andor, int8_t lr)
{
	if (andor->type == AND_IF)
		return (lr == SUCCESS ? 1 : 0);
	if (andor->type == OR_IF)
		return (lr == SUCCESS ? 0 : 1);
	return (FAILURE);
}

uint8_t		lvl_simple_cmd(t_cfg *shell, t_list *s_cmd, char *cmd, uint8_t fg)
{
	uint8_t		ret_job;
	t_job		job;

	cmd_to_job(shell, &job, s_cmd, cmd);
	if ((job.fg = fg))
		set_termios(TCSADRAIN, &job.term_eval);
	ret_job = run_job(shell, &job, job.process);
	routine_clean_job(&job, sizeof(t_job));
	return (ret_job);
}

uint8_t		lvl_and_or(t_cfg *shell, t_list *lst)
{
	uint8_t		ret_job;
	t_and_or	*andor;

	if (!lst)
		return (0);
	andor = lst->data;
	ret_job = lvl_simple_cmd(shell, andor->s_cmd, andor->str, !andor->background);
	if (shell->debug)
		ft_dprintf(shell->debug, "job return [%d] \n", ret_job);
	if (lst->next && condition_respected(andor, ret_job))
		lvl_and_or(shell, lst->next); //continue
	else if (lst->next)
		lvl_and_or(shell, lst->next->next);
	return (0);
}

void		lvl_cmd_table(t_cfg *shell, t_list *lst)
{
	t_cmd_table		*cmd;

	while (lst)
	{
		cmd = lst->data;
		lvl_and_or(shell, cmd->and_or);
		lst = lst->next;
	}
}

uint8_t		ft_eval(t_list *cmd_table)
{
	t_cfg *shell;

	shell = cfg_shell();
	if (shell->debug)
		ft_dprintf(shell->debug, "\n\n--------- EVAL ----------\n\n");
	set_signal_ign();
	lvl_cmd_table(shell, cmd_table);
	if (shell->debug)
		ft_dprintf(shell->debug, "---------- EVAL ----------\n\n");
	return (0);
}
