
#include "libft.h"
#include "struct.h"
#include "sh.h"
#include "ft_printf.h"
#include "var.h"

/*
 *	test n1 -eq n2
 *	0 si n1 et n2 sont des int egaux
 */

uint8_t		test_equal_int(char *s1, char *s2)
{
	ft_atoi(s1) == ft_atoi(s2) ?
		return (0) : return (1);
}
