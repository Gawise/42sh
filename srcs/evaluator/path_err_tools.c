#include "libft.h"
#include "exec.h"
#include <sys/stat.h>



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

uint8_t			c_isdir(char *path)
{
	struct stat		buf;

	if (lstat(path, &buf) == SUCCESS)
		if ((S_IFDIR == (S_IFMT & buf.st_mode)))
			return (1);
	return (0);
}
