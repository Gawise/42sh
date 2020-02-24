#ifndef EXEC_H
# define EXEC_H


	#include <stdio.h>				//debug
# include "struct.h"

# define TRUE 1
# define FALSE -1
# define FAILURE -1
# define SUCCES 0

# define LEFT 0x001
# define RIGHT 0x002
# define MID 0x003
# define EXEC 0x004
# define BUILTIN 0x008		/*rajouter un # define PIPE_ON ?*/
# define FUNCTION 0x016
# define LOCATED 0x0032
# define UNFOUND 0x0064

# define ENV 1
# define LOCAL 2 /* usesless si local intern a sa propre list ? */
# define TMP 4

# define RUNNING 0x001  	/* # define PENDING */
# define WAITING 0x002
# define STOPPED 0x004
# define KILLED 0x08
# define FAILED 0x16
# define COMPLETED 0x32


/*		debug	*/
void	ex(char *s);


/*				*/
int		lvl_cmd_table(t_cfg *shell, t_list *lst);
int		routine_set_pipe(t_list *process, t_pipe *fd);
int		do_dup(t_process *p);

int		process_type(t_list *var, t_process *p);

int		run_job(t_cfg *shell, t_job *job, t_list *process);

int		condition_respectee(void);

int		cmd_to_job(t_job *job, t_list *s_cmd);
int		routine_clean_job(t_job *j);


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
