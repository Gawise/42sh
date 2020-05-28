#include "libft.h"
#include "sh.h"
#include "exec.h"
#include <signal.h>

void	kill_job_active(t_list *lst)
{
	t_job	*j;

	while (lst)
	{
		j = lst->data;
		kill(-j->pgid, SIGKILL);
		lst = lst->next;
	}
}

void	exit_routine(t_cfg *shell, uint8_t ret)
{
	kill_job_active(shell->job);
	if (shell->interactive)
		set_termios(TCSADRAIN, &shell->term_origin);
	clean_cfg(shell);
	exit(ret);
}
