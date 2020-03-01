#include "libft.h"
#include "exec.h"
#include "parser.h"

void	term_create_eval(struct termios *origin, struct termios *eval)
{
	/*
	 *if (!(eval = malloc(sizeof(struct termios))))
	 *    ex("[TERM CREATE EVAL] ERROR MALLOC");
	 */
	ft_memcpy(eval, origin, sizeof(struct termios));
	eval->c_lflag &= ~TOSTOP;
}

int		cmd_to_process(t_list **lst, t_list *s_cmd)
{
	int				i;
	t_process		process;
	t_simple_cmd	*cmd;

	i = 0;
	ft_bzero(&process, sizeof(t_process));
	while (s_cmd)
	{
		
		cmd = s_cmd->data;
		process.cmd = ft_strdup(cmd->cmd_name);
		process.av = ft_tabdup(cmd->av);
		process.std[0] = STDIN_FILENO;
		process.std[1] = STDOUT_FILENO;
		process.std[2] = STDERR_FILENO;
		process.status = WAITING;
		ft_lst_push_back(lst, &process, sizeof(process));
		s_cmd = s_cmd->next;
		i++;
	}
	return (i);   /*i useless sans create_pipe_list ???*/
}

int		cmd_to_job(t_cfg *shell, t_job *job, t_list *s_cmd)
{
	int		nb_p;

	ft_bzero(job, sizeof(t_job));   /*set t_pipe a 0. Besoin d etre a -1?*/
	nb_p = cmd_to_process(&job->process, s_cmd);
	job->std[0] = STDIN_FILENO;
	job->std[1] = STDOUT_FILENO;
	job->std[2] = STDERR_FILENO;
	term_create_eval(shell->term_origin, &job->term_eval);
//	job->pipe = create_pipe_list(nb_p);

	return (0);
}
