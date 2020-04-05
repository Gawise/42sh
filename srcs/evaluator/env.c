#include "libft.h"
#include "exec.h"
#include "struct.h"
#include "sh.h"
#include "var.h"
#include "ft_printf.h"


/*								NEED UPDATE
 *#include "ft_printf.h"
 *
 *void			ft_print_env_lst(t_list *env)
 *{
 *    while (env)
 *    {
 *        ft_dprintf(STDERR_FILENO, "%s=%s\n", ((t_var *)(env->data))->ctab[0],
 *                ((t_var *)(env->data))->ctab[1]);
 *        env = env->next;
 *    }
 *}
 *
 *int			env_creat_lst_env(char **input, t_list **new, int ac)
 *{
 *    char	*tmp;
 *    int		i;
 *
 *    i = ac;
 *    while (input[ac] && ft_strchr(input[ac], '='))
 *        ac++;
 *    if (ac == i)
 *        return (ac);
 *    tmp = input[ac];
 *    input[ac] = NULL;
 *    ft_list_var_env(new, input + i);
 *    input[ac] = tmp;
 *    return (ac);
 *}
 *
 *
 *
 *int			ft_env(t_msh *msh)
 *{
 *    t_msh	fresh;
 *    int		ac;
 *    int		ret;
 *    char	*tmp;
 *    int		i;
 *
 *    ac = 1;
 *    i = 0;
 *    ft_init_struct(&fresh, NULL);
 *    if (!msh->input[1])
 *        return (ft_print_env_lst(msh->env_var));
 *    while ((ret = ft_getopt(&ac, &i, msh->input, "i")) != -1)
 *        if (ret == '?')
 *            return (env_erro(msh->input[ac][i]));
 *    ret = env_creat_lst_env(msh->input, &(fresh.env_var), ac);
 *    if (ac == 1)
 *        ft_lst_join(var_list_cpy(msh->env_var), &(fresh.env_var), 1);
 *    tmp = ft_ctab_to_str((msh->input + ret));
 *    if (!(msh->input[ret]))
 *        ft_print_env_lst(fresh.env_var);
 *    else
 *        i = msh_road(&fresh, tmp);
 *    ft_strdel(&tmp);
 *    ft_clear_msh(&fresh, 1);
 *    return (i);
 *}
 */


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

static uint8_t	check_opt(t_process *p, t_process *p_cpy, int32_t *ac)
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
	if (!opt)
		p_cpy->env = ft_lstdup(p->env, p->env->size, cpy_var_list);
	return (SUCCESS);
}

uint8_t			add_new_var(t_process *p, t_process *p_cpy, int32_t *ac)
{

	while (ft_strchr(p->av[*ac], '='))
	{
		if (several_setvar(&p_cpy->env, p->av[*ac]) == FAILURE)
			ft_dprintf(STDERR_FILENO, "'%s': Not a valid identifier\n", p->av[*ac]);
		*ac += 1;
	}

	return (0);
}

uint8_t			ft_env(t_job *j, t_process *p)
{
	int32_t		ac;
	t_process	p_cpy;
	t_job		j_cpy;

	(void)j;
	ft_bzero(&p_cpy, sizeof(t_process));
	ft_bzero(&j_cpy, sizeof(t_job));
	ac = 1;
	if (check_opt(p, &p_cpy, &ac) == FAILURE)
		return (FAILURE);
	add_new_var(p, &p_cpy, &ac);
	if (!p->av[ac])
		ft_print_env_lst(p_cpy.env);
	else
	{
		p_cpy.av = ft_tabdup(p->av+ac);
		printf("\n la = %s\n", *p_cpy.av);
	}


	return (SUCCESS);
}
