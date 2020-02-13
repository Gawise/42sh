#include "libft.h"
#include "ft_printf.h"
#include "lexer.h"

void	l_set_flag(t_lexer *lexer, t_lexer_flag flag)
{
	lexer->flags |= (1 << flag);
}

void	l_unset_flag(t_lexer *lexer, t_lexer_flag flag)
{
	lexer->flags &= ~(1 << flag);
}

int	l_get_flag(t_lexer *lexer, t_lexer_flag flag)
{
	return (lexer->flags & (1 << flag));
}

int	l_do_nothing(t_lexer *lexer, char c)
{
	(void)lexer;
	(void)c;
	return (1);
}
