#include "libft.h"
#include "ft_printf.h"
#include "lexer.h"

int	l_build_inhib(t_lexer *lexer, char c)
{
	if (lexer->state == S_TK_START)
	{
		if (!l_create_token(lexer))
			return (0);
	}
	if (c == '\"')
		l_add_flag(lexer, c);
	else if (c == '\'')
		l_add_flag(lexer, c);
	else if (c == '\\')
		l_add_flag(lexer, c);
	l_buffer_add(lexer, c);
	lexer->state = S_FLAG;
	lexer->curr_token->type = WORD;
	return (1);
}

int	l_inhib_add(t_lexer *lexer, char c)
{
	if (l_get_flag(lexer, F_BSLASH))
		l_bslash_add(lexer, c);
	else if (l_get_flag(lexer, F_DQUOTE))
		l_dquote_add(lexer, c);
	else if (l_get_flag(lexer, F_SQUOTE))
		l_buffer_add(lexer, c);
	lexer->state = S_FLAG;
	return (1);
}

int	l_delim_inhib(t_lexer *lexer, char c)
{
	l_delim_token(lexer, c);
	lexer->state = S_TK_START;
	if (!l_build_inhib(lexer, c))
		return (0);
	return (1);
}
