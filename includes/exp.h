#ifndef EXP_H
# define EXP_H
# define EXP_BSIZE 255
# include <pwd.h>

typedef struct		s_exp
{
	char		buf[EXP_BSIZE];
	int		i;
	char		*res;
	int		quote;
	int		start;
	int		end;
}			t_exp;




#endif
