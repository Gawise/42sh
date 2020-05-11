#ifndef EXEC_H
# define EXEC_H





# include <stdio.h>				//debug
# include "struct.h"
# include "signal.h"


# define PIPE_ON 1
# define EXEC 2
# define BUILTIN 4		/*rajouter un # define PIPE_ON ?*/
# define SLASH 8
# define R_ERROR 16
# define P_ERROR 32
# define E_BADFD 64
# define E_UNFOUND 128
# define E_ISDIR 256
# define E_NOENT 512
# define E_ACCES 1024
# define E_LOOP 2048
# define E_NTL 4096		/*       1000001000000 */

# define B_SPECIAL 8192		/* 0000.10000000000000 */
# define B_ECHO 0			/* 0000.00000000000000 */
# define B_CD 16384			/* 0001.00000000000000 */
# define B_ENV 32768		/* 0010.00000000000000 */
# define B_SETENV 49152		/* 0011.00000000000000 */
# define B_UNSETENV 65536 	/* 0100.00000000000000 */
# define B_HASH 81920		/* 0101.00000000000000 */
# define B_EXIT 106496		/* 0110.10000000000000 */
# define B_JOBS 114688		/* 0111.00000000000000 */
# define B_FG 131072		/* 1000.00000000000000 */
# define B_BG 147456		/* 1001.00000000000000 */
# define B_TYPE 163840		/* 1010.00000000000000 */

/*   Special Builtin 
# define B_EXIT
# define B_EXPORT
# define B_SET
# define B_UNSET
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

# define ERROR 96			/* P_ERROR | R_ERRO */

/*		debug	*/


void	print_fd_lst(t_list *fd);
void	job_redir(t_list *process);

/*		BUILTIN		*/

uint8_t		ft_echo(t_job *j, t_process *p);
uint8_t		ft_type(t_job *j, t_process *p);
uint8_t		ft_setenv(t_job *j, t_process *p);
uint8_t		ft_unsetenv(t_job *j, t_process *p);
uint8_t		ft_env(t_job *j, t_process *p);
uint8_t		ft_exit(t_job *j, t_process *p);
uint8_t		ft_hash(t_job *j, t_process *p);
uint8_t 	ft_jobs(t_job *j, t_process *p);
uint8_t 	ft_cd(t_job *j, t_process *p);
char		*ft_strcut(char *str, char *delim, unsigned int field);
int			check_whole_path(char *path);
char		cd_getopt(char **str, int *i);
int			cd_home(t_job *job, t_process *p);
char		*cd_setcurpath(t_list **env, char *opr);
int			cd_logically(t_list **env, char *curpath, char *opr);
char		*ft_strrep(char *str, char *rem, char *rep);
char		*ft_pathjoin(char *str1, char *str2);
char		*ft_strcut(char *str, char *delim, unsigned int field);
char		*cd_del_dotcomponents(char *curpath, char *opr);
int			display_cd_errors(char *error);
int			check_chdir_errors(char **error, char *path, char *opr);
int			cd_change_directory(t_list **env, char *curpath, char *opr, char *pwd);
uint8_t		get_job_id(char *ope);
int			print_job_ope(char opt, t_job *j, int8_t ope);
void		print_jobs(char opt, t_job *);
void		print_jobs_opt(t_job *);
char		get_curr(t_list *job, uint8_t stop);
int8_t		find_stopped_job(t_list *job);
int			str_is_digit(char *str);



/*		PROCESS HANDLING	*/
int32_t		has_running(t_list *lst);
int32_t		has_stopped(t_list *lst);
void		with_slash(t_process *p, uint32_t *err);
uint32_t	builtin_search(t_process *p);


/*		REDIR		 */
uint32_t		process_redir(t_process *p, t_list *redir);
void		do_my_dup2(int16_t fd1, int16_t fd2);
uint8_t		bad_fd(int fd);
uint32_t	path_gearing(t_redir *r, char **path, int right);

void		add_fd_process(t_list **fd, int16_t source, int16_t target);
void		do_redir(t_list *fd);
uint8_t		patch_redir(t_list *process);



/*		TERMIOS		*/
void		term_create_eval(struct termios *origin, struct termios *eval);
void		set_termios(int32_t mode, struct termios *term);

/*		ERROR HANDLING	*/
uint32_t		redir_errors_handling(t_process *p, uint32_t error, char *info, int32_t fd);
uint8_t		process_errors_handling(t_process *p);

/*		WAIT		*/
void		wait_process(t_job *job);

/*	 	STEP		*/
uint8_t		ft_eval(t_list *cmd_table);
void		lvl_cmd_table(t_cfg *shell, t_list *lst);
void		cmd_to_job(t_cfg *shell, t_job *job, t_list *s_cmd, char *cmd);
uint8_t		run_job(t_cfg *shell, t_job *job, t_list *process);
//void		run_process(t_job *j, t_process *p);
void		routine_process(t_cfg *shell, t_list *process, t_pipe *fd);
void		process_assign(t_cfg *shell, t_process *p, t_list *assignment);
void		do_pipe(t_process *p);
void		process_type(t_process *p);
void		routine_clean_job(void *del, size_t u);
void		del_struct_process(void *del, size_t u);
void		del_struct_job(void *del, size_t u);

/*		DEBUG */
void	debug_print_all(t_job *j, t_list *process, char *where);
void	debug_print_process(t_job *j, t_process *p, char *where);

#endif
