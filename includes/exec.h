#ifndef EXEC_H
# define EXEC_H


	#include <stdio.h>				//debug
# include "struct.h"
# include "signal.h"

# define SUCCESS 0
# define FAILURE 1
# define TRUE 1
# define FALSE -1

# define LEFT 1
# define RIGHT 2
# define MID 3
# define EXEC 4
# define BUILTIN 8		/*rajouter un # define PIPE_ON ?*/
# define SLASH 16
# define FUNCTION 32
# define ERROR 64
# define E_UNFOUND 192
# define E_ISDIR 320
# define E_NOENT 576
# define E_ACCES 1088
# define E_LOOP 2112
# define E_NTL	4160	/*name too long*/






# define ENV 1
# define LOCAL 2 /* usesless si local intern a sa propre list ? */
# define TMP 4

# define RUNNING 1  	/* # define PENDING */
# define WAITING 2
# define STOPPED 4
# define KILLED 8
# define FAILED 16
# define COMPLETED 32


/*		debug	*/


/*				*/

int		lvl_cmd_table(t_cfg *shell, t_list *lst);
int		routine_set_pipe(t_list *process, t_pipe *fd);
int		do_dup(t_process *p);

void	process_type(t_list *var, t_process *p);

int		run_job(t_cfg *shell, t_job *job, t_list *process);

int		cmd_to_job(t_cfg *shell, t_job *job, t_list *s_cmd, char *cmd);
int		routine_clean_job(t_job *j);


char		*create_abs_path(char *s);
int			path_errors(char *path, uint8_t check_it);
uint8_t		c_enametoolong(char *path);
uint8_t		c_isdir(char *path);
/*	wait */

int		wait_process(t_job *job);

/*		term*/

void		set_termios(struct termios *term);

/* redir */

int		process_redir(t_process *p, t_list *redir);



int		ft_eval(t_list *cmd_table);


#endif
