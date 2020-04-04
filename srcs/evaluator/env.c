#include "libft.h"
#include "exec.h"
#include "struct.h"
#include "sh.h"
#include "var.h"
#include "ft_printf.h"


static uint8_t		env_erro(char c)
{
    char	*usage;

    usage = " : env [-i] [utility [argument ...]]\n";
    ft_dprintf(2, "env: illegal option -- %c\nusage%s", c, usage);
	return (FAILURE);
}


static uint8_t		ft_print_env_lst(t_list *lst)
{ //go dans tools_var.c
	t_var	*env;

	while (lst)
	{
		env = lst->data;
		ft_printf("%s=%s\n", env->ctab[0], env->ctab[1]);
		lst = lst->next;
	}
	return (SUCCESS);
}

static uint8_t	check_opt(t_process *p, t_cfg *shell_cpy, int32_t *ac)
{
	int32_t		i;
	int8_t		ret;
	int8_t		opt;

	opt = 0;
	i = 0;
	while ((ret = ft_getopt(ac, &i, p->av, "i")) != -1)
	{
		if (ret == '?')
			return (env_erro(p->av[*ac][i]));
		if (ret == 'i')
			opt = 1;
	}
	if (!opt && p->env)
		shell_cpy->env = ft_lstdup(p->env, p->env->size, cpy_var_list);
	return (SUCCESS);
}

uint8_t			add_new_var(t_process *p, t_cfg *shell_cpy, int32_t *ac)
{

	while (ft_strchr(p->av[*ac], '='))
	{
		if (several_setvar(&shell_cpy->env, p->av[*ac]) == FAILURE)
			ft_dprintf(STDERR_FILENO, "'%s': Not a valide identifier\n", p->av[*ac]);
		*ac += 1;
	}

	return (0);
}

uint8_t			ft_env(t_job *j, t_process *p)
{
	int32_t		ac;
	t_process	p_cpy;
	t_job		j_cpy;
	t_cfg		shell_cpy;
	t_cfg		*shell;

	(void)j;
	ft_bzero(&p_cpy, sizeof(t_process));
	ft_bzero(&j_cpy, sizeof(t_job));
	ft_bzero(&shell_cpy, sizeof(t_cfg));
	ac = 1;
	if (check_opt(p, &shell_cpy, &ac) == FAILURE)
		return (FAILURE);
	add_new_var(p, &shell_cpy, &ac);
	if (!p->av[ac])
		ft_print_env_lst(shell_cpy.env);
	else
	{
		shell = cfg_shell();
		p_cpy.cmd = ft_strdup(*(p->av+ac));
		p_cpy.av = ft_tabdup(p->av+ac);
		p_cpy.std[0] = -1;
		p_cpy.std[1] = -1;
		p_cpy.std[2] = -1;
		j_cpy.cmd = ft_strdup(j->cmd);
		j_cpy.pgid = j->pgid;
		j_cpy.std[0] = -1;
		j_cpy.std[1] = -1;
		j_cpy.std[2] = -1;
		j_cpy.fg = 1;
		ft_lst_push_back(&j_cpy.process, &p_cpy, sizeof(t_process));
		ft_memcpy(&shell_cpy.term_origin, &shell->term_origin, sizeof(struct termios));
		shell_cpy.pid = shell->pid;
		shell_cpy.intern = ft_lstdup(shell->intern, shell->intern->size, cpy_var_list);
		//shell_cpy.job ???
		shell_cpy.interactive = 0;
		shell_cpy.debug = shell->debug;
		run_job(&shell_cpy, &j_cpy, j_cpy.process);
	}


	return (SUCCESS);
}
