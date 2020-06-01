
#include "libft.h"
#include "struct.h"
#include "sh.h"
#include "ft_printf.h"
#include "var.h"

/*
 *	test -z str
 *	0 si str est de length 0
 */ 

uint8_t		test_len_string(char *s1)
{
	ft_strlen(s1) > 0 ?
		return (1) : return (0);
}
