#include "libft.h"
#include "ft_printf.h"
#include "lexer.h"
#include "sh.h"

int	do_lexing(t_lexer *lexer, int (*token_builder[8][11])(t_lexer *, char))
{
	char	c;
	t_lexer_flag	flag;

	while ((c = l_get_char(lexer)))
	{
		flag = l_get_last_flag(lexer);
		if (!token_builder[lexer->state][l_get_char_type(c)](lexer, c))
			return (0);
		if (cfg_shell()->debug)
		{
			if (c == '\n')
				ft_printf("->\t%s\t%20s\t%s\n\n",
						"\\n", get_state_str(lexer), get_flag_name(flag));
			else
				ft_printf("->\t%c\t%20s\t%s\n",
						c, get_state_str(lexer), get_flag_name(flag));
		}
	}
	if (!token_builder[lexer->state][l_get_char_type(c)](lexer, c))
		return (0);
	return (1);
}

int	ft_lexer(char **str, t_lexer *lexer)
{
	int	(*token_builder[8][11])(t_lexer *, char);

	lexer->src = str;
	lexer->curr = *str;
	init_lexer_states(token_builder);
	if (!do_lexing(lexer, token_builder))
		return (0);
	while (l_get_last_flag(lexer) || l_get_last_here(lexer))
		if (!do_lexing(lexer, token_builder))
			return (0);
	return (1);
}
