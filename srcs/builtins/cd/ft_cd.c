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
#include "lexer.h"
#include "struct.h"
#include "sh.h"
#include "var.h"
#include "ft_printf.h"
#include <dirent.h>
#include <sys/stat.h>

int		cd_oldpwd(t_job *j, t_process *p)
{
	char	**str;
	char	*oldpwd;
	int		ret;

	oldpwd = find_var_value(p->env, "OLDPWD");
	if (!oldpwd || !*oldpwd)
	{
		ft_dprintf(2, "%s: cd: OLDPWD not set\n", PROJECT);
		return (1);
	}
	if (!(str = (char**)malloc(sizeof(char*) * 3)))
		ft_ex(EXMALLOC);
	str[0] = ft_strdup("cd");
	str[1] = ft_strdup(oldpwd);
	str[2] = NULL;
	tabfree(p->av);
	p->av = str;
	ret = ft_cd(j, p);
	return (ret);
}

uint8_t		ft_cd(t_job *job, t_process *p)
{
	char	opt;
	char	*opr;
	char	*curpath;
	int		i;

	i = 1;
	if (!(opt = cd_getopt(p->av, &i)))
		return (1);
	opr = p->av[i];
	if (!opr)
		return (cd_home(job, p));
	else if (ft_strequ(opr, "-"))
		return (cd_oldpwd(job, p));
	if (!(curpath = cd_setcurpath(&p->env, opr)))
		ft_ex(EXMALLOC);
	if (opt == 'L')
		return (cd_logically(&p->env, curpath, opr));
	return (cd_change_directory(&p->env, curpath, opr, NULL));
}
