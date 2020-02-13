#ifndef EXP_H
# define EXP_H
# define EXP_BSIZE 32

typedef struct		s_exp
{
	char		buf[EXP_BSIZE];
	int		i;
	char		*res;
	int		quote;
}			t_exp;




#endif
