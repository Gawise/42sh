#include "libft.h"
#include "exec.h"
#include "sh.h"
#include <sys/stat.h>
#include <dirent.h>

uint8_t			c_enametoolong(char *path)
{
	int		i;
	//char	*buf;

	i = 2;
	if (ft_strlen(path) >= 1024)
		return (0);
	/*
	 *while ((buf = ft_strcut(path, "/", i)))
	 *{
	 *    if (ft_strlen(buf) >= 1024)
	 *    {
	 *        free(buf);
	 *        return (0);
	 *    }
	 *    if (!*buf)
	 *    {
	 *        free(buf);
	 *        break ;
	 *    }
	 *    free(buf);
	 *    i++;
	 *}
	 */
	return (1);
}

uint8_t			c_enoent(char *path)
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

uint8_t			c_eacces(char *path)
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

uint8_t			c_eloop(char *path)
{
	struct stat		buf;

	if (stat(path, &buf) < 0)
		if (!lstat(path, &buf) && S_IFLNK == (S_IFMT & buf.st_mode))
			return (0);
	return (1);
}

uint32_t		path_errors(char *path, uint8_t check_it)
{
	char			*idx;
	int				ret;

	idx = path;
	ret = 0;
	if (!c_enametoolong(path))
		return (E_NTL);
	if (c_isdir(path) & check_it)
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
