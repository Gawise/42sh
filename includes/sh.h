#ifndef SH_H
# define SH_H

# define NAME_SH "21sh-1.0$ "

typedef struct	s_cfg t_cfg;

void	set_signal_ign(void);
void	init_shell(char **env);
t_cfg		*cfg_shell(void);


#endif
