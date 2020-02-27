#include "libft.h"
#include "exec.h"
#include <sys/stat.h>
#include <dirent.h>

int			c_enoent(char *path)
{
	DIR				*dir;
	struct dirent	*dit;
	int				ret;
	char			*tmp;

	ret = 0;
	if (!*path || !strcmp(path, "/"))
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

int			c_eacces(char *path)
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

int			c_eloop(char *path)
{
	struct stat		buf;

	if (stat(path, &buf) < 0)
		if (!lstat(path, &buf) && S_IFLNK == (S_IFMT & buf.st_mode))
			return (0);
	return (1);
}

int			path_errors(char *path)
{
	char			*idx;
	int				ret;

	idx = path;
	ret = 0;
	if (!c_enametoolong(path))
		return (E_NTL);
	if (c_enotdir(path))
		return (E_ISDIR);
	while (1)
	{
		if ((idx = ft_strchr(idx, '/')))
			*idx = '\0';
		if (!ret && !c_enoent(path))
			ret = E_NOENT;
		if (!ret && !c_eloop(path))
			ret = E_LOOP;
		if (!ret && !c_eacces(path))
			ret = E_ACCES;
		if (idx)
			*(idx) = '/';
		if (!idx || ret)
			return (ret);
		idx++;
	}
}
