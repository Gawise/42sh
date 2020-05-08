#include "libft.h"
#include "exec.h"
#include "parser.h"
#include "var.h"



void	cmd_to_process(t_list **lst, t_list *s_cmd)
{
	t_process		process;
	t_simple_cmd	*cmd;
	uint8_t			nb_redir;

	ft_bzero(&process, sizeof(t_process));
	while (s_cmd)
	{
		
		cmd = s_cmd->data;
		process.cmd = ft_strdup(cmd->cmd_name);
		process.av = ft_tabdup(cmd->av);
		nb_redir = ft_lstcount(cmd->redir);
		process.std = ft_memalloc((nb_redir + 3) * 2);
		process.std[0] = STDIN_FILENO;
		process.std[1] = STDIN_FILENO;
		process.std[2] = STDOUT_FILENO;
		process.std[3] = STDOUT_FILENO;
		process.std[4] = STDERR_FILENO;
		process.std[5] = STDERR_FILENO;
		process.status = WAITING;
		process.redir = cmd->redir;
		process.assign = cmd->assign;
		ft_lst_push_back(lst, &process, sizeof(process));
		s_cmd = s_cmd->next;
	}
	return ;
}

void	cmd_to_job(t_cfg *shell, t_job *job, t_list *s_cmd, char *cmd)
{
	ft_bzero(job, sizeof(t_job));
	cmd_to_process(&job->process, s_cmd);
	job->cmd = ft_strdup(cmd);
	job->std[0] = dup(STDIN_FILENO);
	job->std[1] = dup(STDOUT_FILENO);
	job->std[2] = dup(STDERR_FILENO);
	term_create_eval(&shell->term_origin, &job->term_eval);
}
