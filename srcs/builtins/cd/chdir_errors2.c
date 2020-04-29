/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chdir_errors2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guaubret <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/11 14:29:15 by guaubret          #+#    #+#             */
/*   Updated: 2020/04/11 14:29:16 by guaubret         ###   ########.fr       */
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

int			check_enoent(char *path)
{
	DIR				*dir;
	struct dirent	*dit;
	int				ret;
	char			*tmp;

	ret = 0;
	if (!*path || !ft_strcmp(path, "/"))
		return (1);
	if (!(tmp = ft_strrchr(path, '/')))
		return (0);
	*tmp = '\0';
	if (tmp == path)
		dir = opendir("/");
	else
		dir = opendir(path);
	*tmp = '/';
	if (!dir)
		return (1);
	while (!ret && (dit = readdir(dir)))
		if (!ft_strcmp(dit->d_name, tmp + 1))
			ret = 1;
	closedir(dir);
	return (ret);
}

int			check_eacces(char *path)
{
	struct stat		buf;

	if (!*path)
	{
		if (!lstat("/", &buf))
			if (!(buf.st_mode & S_IXUSR))
				return (0);
	}
	else
	{
		if (!lstat(path, &buf))
			if (!(buf.st_mode & S_IXUSR))
				return (0);
	}
	return (1);
}

int			check_enotdir(char *path)
{
	struct stat		buf;

	if (!lstat(path, &buf))
		if (!(S_IFDIR == (S_IFMT & buf.st_mode)))
			return (0);
	return (1);
}

int			check_eloop(char *path)
{
	struct stat		buf;

	if (stat(path, &buf) < 0)
		if (!lstat(path, &buf) && S_IFLNK == (S_IFMT & buf.st_mode))
			return (0);
	return (1);
}

int			check_whole_path(char *path)
{
	char			*idx;
	int				ret;

	idx = path;
	ret = 0;
	while (1)
	{
		if ((idx = ft_strchr(idx, '/')))
			*idx = '\0';
		if (!ret && !check_enoent(path))
			ret = 1;
		if (!ret && !check_eloop(path))
			ret = 2;
		if (!ret && !check_enotdir(path))
			ret = 3;
		if (!ret && !check_eacces(path))
			ret = 4;
		if (idx)
			*(idx) = '/';
		if (!idx || ret)
			return (ret);
		idx++;
	}
}
