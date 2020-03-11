#include "libft.h"
#include "ft_printf.h"
#include "lexer.h"
#include "line_edition.h"
#include "sh.h"

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

int	l_complete_str(t_lexer *lexer,  char c)
{
	size_t	pos;
	char	*new;

	(void)c;
	pos = lexer->curr - lexer->src;
	new = ft_prompt("> ", COLOR_SUBPROMPT);
	if (!(lexer->src = ft_strclnjoin(lexer->src, new)))
	{
		ft_printf("erreur malloc complete_flag\n");
		exit(1);
	}
	lexer->curr = lexer->src + pos;
	return (1);
}
