
#include "libft.h"
#include "struct.h"
#include "sh.h"
#include "ft_printf.h"
#include "var.h"

/*
 *	test n1 -le n2
 *	0 si n1 <= n2
 */

uint8_t		test_lessor_int(char *s1, char *s2)
{
	ft_atoi(s1) <= ft_atoi(s2) ?
		return (0) : return (1);
}
