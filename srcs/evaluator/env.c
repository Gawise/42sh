/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmerieux <hmerieux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/31 18:00:30 by hmerieux          #+#    #+#             */
/*   Updated: 2020/02/10 17:02:29 by hmerieux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */




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
 *static int	env_erro(char c)
 *{
 *    char	*usage;
 *
 *    usage = " : env[-i] [utility [argument ...]]\n";
 *    ft_dprintf(2, "env: illegal option -- %c\nusage%s", c, usage);
 *    return (1);
 *}
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
 *        ft_lst_join(env_list_cpy(msh->env_var), &(fresh.env_var), 1);
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
