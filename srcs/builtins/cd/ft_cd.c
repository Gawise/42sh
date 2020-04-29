/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guaubret <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/11 14:29:27 by guaubret          #+#    #+#             */
/*   Updated: 2020/04/11 14:29:28 by guaubret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "exec.h"
#include "struct.h"
#include "sh.h"
#include "var.h"
#include "ft_printf.h"
#include <dirent.h>
#include <sys/stat.h>

int		cd_oldpwd(t_list **env, t_job *j, t_process *p)
{
	char	**str;
	char	*oldpwd;
	int		ret;

	oldpwd = find_var_value(*env, "OLDPWD");
	if (!oldpwd || !*oldpwd)
	{
		ft_dprintf(2, "minishell: cd: OLDPWD not set\n");
		return (1);
	}
	if (!(str = (char**)malloc(sizeof(char*) * 3)))
		exit(EXIT_FAILURE);
	str[0] = ft_strdup("cd");
	str[1] = ft_strdup(oldpwd);
	str[2] = NULL;
	ret = ft_cd(j, p);
	tabfree(str);
	return (ret);
}

uint8_t		ft_cd(t_job *job, t_process *p)
{
	char	opt;
	char	*opr;
	char	*curpath;
	int		i;

	i = 1;
	if (!(opt = cd_getopt(str, &i)))
		return (1);
	opr = str[i];
	if (!opr)
		return (cd_home(&p->env));
	else if (ft_strequ(opr, "-"))
		return (cd_oldpwd(&p->env, j, p));
	if (!(curpath = cd_setcurpath(&p->env, opr)))
		exit(EXIT_FAILURE);
	if (opt == 'L')
		return (cd_logically(&p->env, curpath, opr));
	return (cd_change_directory(&p->env, curpath, opr, NULL));
}
