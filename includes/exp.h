#ifndef EXP_H
# define EXP_H
# define EXP_BSIZE 50
# include <pwd.h>

typedef struct		s_exp
{
	char		buf[EXP_BSIZE];
	int		i;
	int		quote;
	int		bs;
	char		*param;
	char		*word;
	char		*res;
}			t_exp;




#endif
