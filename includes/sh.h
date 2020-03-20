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

void	ex(char *s); //pour la gestion des erreurs amenant a quiter

void	set_signal_ign(void);
uint8_t	init_shell(char **env, char **av);
t_cfg	*init_cfg(char **env, char **av);
t_cfg	*cfg_shell(void);

#endif
