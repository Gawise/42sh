#ifndef SH_H
# define SH_H

# define SUCCESS			0
# define FAILURE			1
# define TRUE				1
# define FALSE				-1

# define FD_NONINT			259
# define FD_DEBUG			260

# define PROJECT			"21sh"
# define NAME_SH			"21sh-1.0$ "
# define COLOR_SH			"\e[0;32m"
# define COLOR_SUBPROMPT	"\e[0;31m"
# define USAGE				"Usage: 21sh [-d [path]] [file]\nexit\n"

/*
**********************************
************ ERROR ***************
**********************************
*/
# define STR_UNFOUND 		"Command not found"
# define STR_ISDIR			"Is a directory"
# define STR_ISNDIR			"Is not a directory"
# define STR_NOENT			"No such file or directory"
# define STR_ACCES			"Permission denied"
# define STR_LOOP			"Too many links"
# define STR_NTL			"File name too long"

/*
**********************************
************ FT_EX ***************
**********************************
*/
# define EX					"[Critical systeme error]\nexit\n"
# define EXMALLOC			"[Critical error] Cannot allocate memory\nexit\n"
# define EXEXEC				"[Critical Error] Execve fail\nexit\n"
# define EXFD				"[Critical Error] Bad file descriptor\nexit\n"
# define EXFD2				"Bad file descriptor\n"
# define EXUEPTD			"Unexpected error occurred... Shall leave\nexit\n"

# include <stdint.h>
# include <stddef.h>

typedef struct s_cfg	t_cfg;

void		set_var(t_cfg *shell);
void		exit_routine(t_cfg *shell, uint8_t ret);
void		ft_ex(char *error);
void		protect_malloc(void *malloc);
void		clean_cfg(t_cfg *shell);
void		del_struct_tvar(void *del, size_t u);
void		set_signal_ign(void);
t_cfg		*init_shell(char **env, char **av, int ac);
t_cfg		*init_cfg(char **env, char **av, int ac);
t_cfg		*cfg_shell(void);
uint32_t	path_errors(char *path, uint8_t check_dir, uint32_t right);
uint8_t		c_enametoolong(char *path);
uint8_t		c_isdir(char *path);
uint8_t		c_eacces(char *path, uint32_t right);
uint8_t		c_eloop(char *path);
uint8_t		c_enoent(char *path);
char		*create_abs_path(char *s);
uint32_t	check_access(char *path, int right);
void		set_signal_child(void);
void		set_signal_ign(void);

#endif
