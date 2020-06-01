
#include "libft.h"
#include "struct.h"
#include "sh.h"
#include "ft_printf.h"
#include "var.h"

/*
 *	test s1 = s2
 *	0 si s1 et s2 sont identiques (strequ)
 */ 

uint8_t		test_equal_string(char *s1, char *s2)
{
	ft_strequ(s1, s2) == 0 ?
		return (1) : return (0);
}
