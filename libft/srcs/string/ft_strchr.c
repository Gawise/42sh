#include <stdlib.h>

char	*ft_strchr(const char *s, int c)
{
	char *str;

	str = (char *)s;
	while (str && *str != c)
	{
		if (*str == '\0')
			return (NULL);
		str++;
	}
	return (str);
}
