#ifndef EXEC_H
# define EXEC_H


	#include <stdio.h>				//debug
# include "struct.h"

# define SUCCESS 0
# define TRUE 1
# define FALSE -1
# define FAILURE -1

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
void	ex(char *s);


/*				*/
int		lvl_cmd_table(t_cfg *shell, t_list *lst);
int		routine_set_pipe(t_list *process, t_pipe *fd);
int		do_dup(t_process *p);

uint8_t		process_type(t_list *var, t_process *p);

int		run_job(t_cfg *shell, t_job *job, t_list *process);

int		cmd_to_job(t_job *job, t_list *s_cmd);
int		routine_clean_job(t_job *j);


int			path_errors(char *path);
uint8_t		c_enametoolong(char *path);
uint8_t		c_enotdir(char *path);
/*	wait */

int		wait_process(t_job *job);



/*	env	*/

int		ft_setenv(t_list **lst, char *var, char *value, uint8_t overw);
char	*ft_getenv(t_list *env, char *name);
void	create_lst_env(t_list **lst, char **env);
char	**create_tab_env(t_list *list, int count);
t_list	*setenv_find(t_list *lst, char *var);
t_list		*env_list_cpy(t_list *lst);


/* externe a l exec */
void	init_shell(t_cfg *cfg, char **env);
int		ft_eval(t_cfg *shell, t_list *cmd_table);


#endif
