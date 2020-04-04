#ifndef EXEC_H
# define EXEC_H





	#include <stdio.h>				//debug
# include "struct.h"
# include "signal.h"


# define PIPE_ON 2
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

# define B_ECHO 8192		/* 0000.10000000000000 */
# define B_EXIT 24576		/* 0001.10000000000000 */
# define B_CD 40960			/* 0010.10000000000000 */
# define B_ENV 57344		/* 0011.10000000000000 */
# define B_SETENV 73728		/* 0100.10000000000000 */
# define B_UNSETENV 90112 	/* 0101.10000000000000 */
# define B_HASH 106496		/* 0110.10000000000000 */

/*
# define B_EXIT 16384
# define B_CD 32768
# define B_ENV 65536
# define B_SETENV 131072
# define B_UNSETENV 262144
*/


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



/*		BUILTIN	*/

uint8_t			ft_setenv(t_job *j, t_process *p);
uint8_t			ft_unsetenv(t_job *j, t_process *p);
uint8_t			ft_env(t_job *j, t_process *p);
uint8_t			ft_exit(t_job *j, t_process *p);



/*		TERMIOS		*/

void	term_create_eval(struct termios *origin, struct termios *eval);

/*				*/

int		lvl_cmd_table(t_cfg *shell, t_list *lst);
int		routine_process(t_cfg *shell, t_list *process, t_pipe *fd);
int		do_pipe(t_process *p);

void	process_type(t_process *p);

int		run_job(t_cfg *shell, t_job *job, t_list *process);

int		cmd_to_job(t_cfg *shell, t_job *job, t_list *s_cmd, char *cmd);
int		routine_clean_job(t_job *j);

void	run_process(t_job *j, t_process *p);

char		*create_abs_path(char *s);
int			path_errors(char *path, uint8_t check_it);
uint8_t		c_enametoolong(char *path);
uint8_t		c_isdir(char *path);
/*	wait */

int		wait_process(t_job *job);

/*		term*/

void		set_termios(int32_t mode, struct termios *term);

/* redir */

int		process_redir(t_process *p, t_list *redir);
void	do_my_dup2(int8_t fd1, int8_t fd2);


int		ft_eval(t_list *cmd_table);


#endif
