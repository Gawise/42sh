#ifndef EXEC_H
# define EXEC_H

# include "struct.h"

# define ERROR					48

# if __APPLE__
#  define SAVE_IN               252
#  define SAVE_OUT              253
#  define SAVE_ERR              254
# else
#  define SAVE_IN				256
#  define SAVE_OUT				257
#  define SAVE_ERR				258
# endif

/*
*****************************************************
******************* Process SETUP *******************
*****************************************************
*/
# define PIPE_ON				1
# define EXEC					2
# define BUILTIN				4
# define NOCMD					8
# define R_ERROR				16
# define P_ERROR				32
# define E_BADFD				64
# define E_UNFOUND				128
# define E_ISDIR				256
# define E_NOENT				512
# define E_ACCES				1024
# define E_LOOP					2048
# define E_NTL					4096
# define B_SPECIAL				8192
# define B_ECHO					0
# define B_CD					16384
# define B_ENV					32768
# define B_SETENV				49152
# define B_UNSETENV				65536
# define B_HASH					81920
# define B_EXIT					106496
# define B_JOBS					114688
# define B_FG					131072
# define B_BG					147456
# define B_TYPE					163840

/*
*****************************************************
***************** Process/Job STATUS ****************
*****************************************************
*/
# define RUNNING				1
# define WAITING				2
# define STOPPED				4
# define KILLED					8
# define FAILED					16
# define COMPLETED				32

/*
*****************************************************
***************** Handler SIGNAL ********************
*****************************************************
*/
# define S_SIGHUP				"Hangup"
# define S_SIGINT				""
# define S_SIGQUIT				"Quit"
# define S_SIGILL				"Illegal instruction"
# define S_SIGTRAP				"Trace/Breakpoint trap"
# define S_SIGABRT				"Aborted"
# define S_SIGBUS				"Bus Error"
# define S_SIGFPE				"Floating point exception"
# define S_SIGKILL				"Killed"
# define S_SIGUSR1				"User defined signal 1"
# define S_SIGSEGV				"Segmentation fault"
# define S_SIGUSR2				"User defined signal 2"
# define S_SIGALRM				"Alarm clock"
# define S_SIGTERM				"Terminated"
# define S_SIGXCPU				"CPU time limit exceeded"
# define S_SIGXFSZ				"File size limit exceeded"
# define S_SIGPROF				"Profiling timer expired"
# define S_SIGSYS				"Bad system call"
# define S_SIGTTOU				"Stopped(SIGTTOU)"
# define S_SIGTTIN				"Stopped(SIGTTIN)"
# define S_SIGTSTP				"Stopped(SIGTSTP)"
# define S_SIGSTOP				"Stopped(SIGSTOP)"

/*
*****************************************************
******************* BUILTIN *************************
*****************************************************
*/

uint8_t			ft_bg(t_job *j, t_process *p);
uint8_t			ft_fg(t_job *j, t_process *p);
uint8_t			ft_echo(t_job *j, t_process *p);
uint8_t			ft_type(t_job *j, t_process *p);
uint8_t			ft_setenv(t_job *j, t_process *p);
uint8_t			ft_unsetenv(t_job *j, t_process *p);
uint8_t			ft_env(t_job *j, t_process *p);
uint8_t			ft_exit(t_job *j, t_process *p);
int8_t			protect_job(int8_t update);
uint8_t			ft_hash(t_job *j, t_process *p);
uint8_t			ft_jobs(t_job *j, t_process *p);
uint8_t			ft_cd(t_job *j, t_process *p);
char			*ft_strcut(char *str, char *delim, unsigned int field);
int				check_whole_path(char *path);
char			cd_getopt(char **str, int *i);
int				cd_home(t_job *job, t_process *p);
char			*cd_setcurpath(t_list **env, char *opr);
int				cd_logically(t_list **env, char *curpath, char *opr);
char			*ft_strrep(char *str, char *rem, char *rep);
char			*ft_pathjoin(char *str1, char *str2);
char			*ft_strcut(char *str, char *delim, unsigned int field);
char			*cd_del_dotcomponents(char *curpath, char *opr);
int				display_cd_errors(char *error);
int16_t			get_job_id(char *ope);
int				print_job_ope(char opt, t_job *j, int8_t ope);
void			print_jobs(char opt, t_job *j);
void			print_jobs_opt(t_job *j);
char			get_curr(t_list *job, uint8_t stop);
int8_t			find_stopped_job(t_list *job);
int				str_is_digit(char *str);
int				check_chdir_errors(char **error, char *path,
									char *opr);
int				cd_change_directory(t_list **env, char *curpath,
									char *opr, char *pwd);
void			hash_usage_error(void);
int				hash_not_found(char *cmd);

/*
*****************************************************
******************* TERMIOS *************************
*****************************************************
*/
void			term_create_eval(struct termios *origin, struct termios *eval);
void			set_termios(int32_t mode, struct termios *term);

/*
*****************************************************
******************* JOB *****************************
*****************************************************
*/
uint8_t			ft_eval(t_list *cmd_table);
void			cmd_to_job(t_cfg *shell, t_job *job, t_list *s_cmd, char *cmd);
uint8_t			run_job(t_cfg *shell, t_job *job, t_list *process);
void			wait_process(t_job *job);
t_list			*find_job_by_status(t_list *lst, uint8_t want);
void			builtin_save_fd(t_job *j);
void			builtin_restor_fd(t_job *j);

/*
*****************************************************
****************** PROCESS **************************
*****************************************************
*/

uint8_t			builtin_process(t_job *j, t_process *p);
void			routine_process(t_cfg *shell, t_list *process, t_pipe *fd);
void			run_process(t_cfg *shell, t_job *j, t_process *p);
void			process_type(t_process *p);
void			process_assign(t_cfg *shell, t_process *p, t_list *assignment);
void			do_pipe(t_process *p);
int32_t			has_running(t_list *lst);
int32_t			has_stopped(t_list *lst);
void			any_slash(t_list *env, t_process *p, uint32_t *err);
void			with_slash(t_process *p, uint32_t *err);
uint32_t		builtin_search(t_process *p);
t_process		*find_process_by_status(t_list *lst, uint8_t want);
t_process		*find_process_by_pid(t_list *lst, pid_t child);

/*
*****************************************************
****************** REDIR & FD ***********************
*****************************************************
*/
uint32_t		process_redir(t_process *p, t_list *redir);
void			do_my_dup2(int16_t fd1, int16_t fd2);
uint8_t			bad_fd(int fd);
uint32_t		path_gearing(t_redir *r, char **path, int right);
uint8_t			add_fd_process(t_list **fd, int16_t source, int16_t target);
void			do_redir(t_list *fd);
uint8_t			patch_redir(t_list *process);
void			close_fd(t_list *lst);
void			job_redir(t_list *process);
void			do_redir(t_list *fd);
int32_t			create_fd_null(void);
uint32_t		right_fifo(char *path, uint32_t *right);

/*
*****************************************************
**************** RERROR HANDLIN *********************
*****************************************************
*/
uint32_t		redir_errors_handling(t_process *p, uint32_t error,
										char *info, int32_t fd);
uint32_t		process_errors_handling(t_process *p, uint32_t	err);

/*
*****************************************************
***************** SIGNAL ****************************
*****************************************************
*/
char			**create_message_signal(char **tab);
uint8_t			print_message_signal(uint8_t sig, t_job *j, t_process *p);

/*
*****************************************************
*************** DESTRUCTOR **************************
*****************************************************
*/
void			routine_clean_job(void *del, size_t u);
void			del_struct_process(void *del, size_t u);
void			del_struct_job(void *del, size_t u);

/*
*****************************************************
***************** DEBUG *****************************
*****************************************************
*/
void			debug_print_job(int32_t fd_debug, t_job *j, char *where);
void			debug_print_all_process(t_list *process, char *where);
void			debug_print_all_job(t_list *job, char *where);
void			debug_print_everything(t_list *job, char *where);
void			debug_print_process(int32_t fd_debug, t_process *p,
									char *where);

#endif
