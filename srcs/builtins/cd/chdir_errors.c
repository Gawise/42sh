/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chdir_errors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guaubret <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/11 14:29:08 by guaubret          #+#    #+#             */
/*   Updated: 2020/04/11 14:29:09 by guaubret         ###   ########.fr       */
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

int			display_cd_errors(char *error)
{
	if (!(error))
		exit(EXIT_FAILURE);
	ft_dprintf(2, "%s: cd: %s", NAME_SH, error);
	free(error);
	return (1);
}

int			check_enametoolong(char *path)
{
	int		i;
	char	*buf;

	i = 2;
	if (ft_strlen(path) >= 1024)
		return (0);
	while ((buf = ft_strcut(path, "/", i)))
	{
		if (ft_strlen(buf) >= 1024)
		{
			free(buf);
			return (0);
		}
		if (!*buf)
		{
			free(buf);
			break ;
		}
		free(buf);
		i++;
	}
	return (1);
}

int			check_chdir_errors(char **error, char *path, char *opr)
{
	int ret;

	ret = 0;
	if (!check_enametoolong(path))
	{
		*error = (ft_strjoin(2, opr, ": File name too long\n"));
		return (1);
	}
	ret = check_whole_path(path);
	if (ret == 1)
		*error = (ft_strjoin(2, opr, ": No such file or directory\n"));
	if (ret == 2)
		*error = (ft_strjoin(2, opr, ": Symbolic link error\n"));
	if (ret == 3)
		*error = (ft_strjoin(2, opr, ": Not a directory\n"));
	if (ret == 4)
		*error = (ft_strjoin(2, opr, ": Permission denied\n"));
	if (!ret)
		return (0);
	return (1);
}
