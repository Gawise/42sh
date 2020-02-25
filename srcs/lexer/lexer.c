#include "libft.h"
#include "ft_printf.h"
#include "lexer.h"

/*
 *	0: Inhibiteurs	" ' \
 *	1: Control	& | ;
 *	2: Redir	< >
 *	3: Newline	\n
 *	4: Chiffres	0-9
 *	5: Expansions	$
 *	6: Delimiter	\t SPACE
 *	7: End of Input \0
 *	8: Brackets	{ }
 *	9: Equal-sign	=
 *	10: Reste
 */

int	lex_err(t_lexer *lexer, char c)
{
	(void)lexer;
	(void)c;
	ft_printf("lexing error\n");
	exit(1);
	return (1);
}

char	l_get_char(t_lexer *lexer)
{
	if (!*(lexer->curr))
		return (0);
	return (*lexer->curr++);
}

t_char_type	l_get_char_type(char c)
{
	if (c == 0)
		return (C_EOI);
	else if (c == '=')
		return (C_EQU);
	else if (c == '\n')
		return (C_NEWLINE);
	else if (c == '$')
		return (C_EXP);
	else if (ft_strchr("\"\'\\", c))
		return (C_INHIBITOR);
	else if (ft_strchr("&|;", c))
		return (C_CONTROL);
	else if (ft_strchr("<>", c))
		return (C_REDIR);
	else if (ft_isdigit(c))
		return (C_DIGIT);
	else if (ft_strchr(" \t", c))
		return (C_BLANK);
	else if (ft_strchr("{}", c))
		return (C_BRACK);
	else
		return (C_OTHER);
}

void	init_lexer_states(int (*token_builder[8][11])(t_lexer *, char))
{
	init_start_state(token_builder);
	init_heredoc_body_state(token_builder);
	init_amp_pipe_state(token_builder);
	init_redir_state(token_builder);
	init_exp_state(token_builder);
	init_word_state(token_builder);
	init_io_number_state(token_builder);
	init_flag_state(token_builder);
}

int	do_lexing(t_lexer *lexer, int (*token_builder[8][11])(t_lexer *, char))
{
	char	c;
	t_lexer_flag	flag;

	while ((c = l_get_char(lexer)))
	{
		flag = l_get_last_flag(lexer);
		if (!token_builder[lexer->state][l_get_char_type(c)](lexer, c))
			lex_err(lexer, c);
		if (c == '\n')
			ft_printf("->\t%s\t%20s\t%s\n\n",
			"\\n", get_state_str(lexer), get_flag_name(flag));
		else
			ft_printf("->\t%c\t%20s\t%s\n",
			c, get_state_str(lexer), get_flag_name(flag));
	}
	if (!token_builder[lexer->state][l_get_char_type(c)](lexer, c))
		lex_err(lexer, c);
	return (1);
}

int	ft_lexer(char *str, t_lexer *lexer)
{
	int	(*token_builder[8][11])(t_lexer *, char);

	lexer->src = str;
	lexer->curr = str;
	init_lexer_states(token_builder);
	do_lexing(lexer, token_builder);
	while (l_get_last_flag(lexer) || l_get_flag(lexer, F_HEREDOC))
		do_lexing(lexer, token_builder);
	return (1);
}
