#include "libft.h"

uint8_t		test_greator_int(char *s1, char *s2)
{
	return ((ft_atoi(s1) >= ft_atoi(s2)) ? 0 : 1);
}
