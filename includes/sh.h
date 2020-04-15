#ifndef SH_H
# define SH_H

# define SUCCESS 0
# define FAILURE 1
# define TRUE 1
# define FALSE -1

# define NAME_SH "21sh-1.0$ "
# define COLOR_SH "\e[0;32m"
# define COLOR_SUBPROMPT "\e[0;31m"

# include <stdint.h>

typedef struct	s_cfg t_cfg;

void		ft_ex(char *error);
void		clean_cfg(t_cfg *shell);
void		set_signal_ign(void);
void		init_shell(char **env, char **av, int ac);
t_cfg		*init_cfg(char **env, char **av, int ac);
t_cfg		*cfg_shell(void);

/*	COMMON TOOLS */
uint32_t	path_errors(char *path, uint8_t check_it);
uint8_t		c_enametoolong(char *path);
uint8_t		c_isdir(char *path);
char		*create_abs_path(char *s);
uint32_t	check_access(char *path, int right);




/*		SIGNAL  */
void		set_signal_child(void);
void		set_signal_ign(void);

#endif
