#ifndef SH_H
# define SH_H

# define NAME_SH "21sh-1.0$ "

# include <stdint.h>

typedef struct	s_cfg t_cfg;

void	ex(char *s); //pour la gestion des erreurs amenant a quiter

void	set_signal_ign(void);
uint8_t	init_shell(char **env, char **av);
t_cfg	*init_cfg(char **env, char **av);
t_cfg	*cfg_shell(void);

#endif
