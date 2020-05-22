#ifndef SH_H
# define SH_H

# define SUCCESS 0
# define FAILURE 1
# define TRUE 1
# define FALSE -1

# define PROJECT "21sh"
# define NAME_SH "21sh-1.0$ "
# define COLOR_SH "\e[0;32m"
# define COLOR_SUBPROMPT "\e[0;31m"
# define USAGE "Usage: 21sh [-d [path]]  [file]\nexit\n"


# define STR_UNFOUND "Command not found"
# define STR_ISDIR "Is a directory"
# define STR_ISNDIR "Is not a directory"
# define STR_NOENT "No such file or directory"
# define STR_ACCES "Permission denied"
# define STR_LOOP "Too many links"
# define STR_NTL "File name too long"		/*       1000001000000 */

# define EX "[Critical Systeme Error]\nexit\n"
# define EXMALLOC "[Critical Error] Cannot allocate memory\nexit\n"
# define EXEXEC "[Critical Error] Execve fail\nexit\n"
# define EXFD "[Critical Error] Bad file descriptor\nexit\n"



# include <stdint.h>
# include <stddef.h>

typedef struct	s_cfg t_cfg;

void		ft_ex(char *error);
void		clean_cfg(t_cfg *shell);
void		del_struct_tvar(void *del, size_t u);
void		set_signal_ign(void);
void		init_shell(char **env, char **av, int ac);
t_cfg		*init_cfg(char **env, char **av, int ac);
t_cfg		*cfg_shell(void);

/*	COMMON TOOLS */
uint32_t	path_errors(char *path, uint8_t check_dir, uint32_t right);
uint8_t		c_enametoolong(char *path);
uint8_t		c_isdir(char *path);
uint8_t		c_eacces(char *path, uint32_t right);
uint8_t		c_eloop(char *path);
uint8_t		c_enoent(char *path);
char		*create_abs_path(char *s);
uint32_t	check_access(char *path, int right);




/*		SIGNAL  */
void		set_signal_child(void);
void		set_signal_ign(void);

#endif
