#include "libft.h"
#include "ft_printf.h"
#include "lexer.h"
#include "sh.h"
#include "exec.h"
#include "var.h"
#include "line_edition.h"

int			lex_err(t_lexer *lexer, char c)
{
	(void)lexer;
	(void)c;
	ft_printf("lexing error\n");
	exit(1);
	return (1);
}

char		l_get_char(t_lexer *lexer)
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

char		*l_get_prompt_prefix(t_lexer *lexer)
{
	t_lexer_flag	flag;
	t_here_queue	*here;

	if ((flag = l_get_last_flag(lexer)))
	{
		if (flag == F_DQUOTE)
			return (ft_strdup("dquote"));
		else if (flag == F_SQUOTE)
			return (ft_strdup("squote"));
		else if (flag == F_BSLASH)
			return (ft_strdup("bslash"));
		else if (flag == F_BRACKEXP)
			return (ft_strdup("braceparam"));
	}
	else if ((here = l_get_last_here(lexer)))
		return (ft_strdup("heredoc"));
	return (NULL);
}

int			l_complete_str(t_lexer *lexer, char c)
{
	size_t	pos;
	char	*new;
	char	*pmt_prefix;

	(void)c;
	new = NULL;
	pos = lexer->curr - *lexer->src;
	if (!(pmt_prefix = l_get_prompt_prefix(lexer))
	|| ft_dprintf(0, "%s", pmt_prefix) == -1
	|| !(new = ft_prompt(find_var_value(cfg_shell()->intern, "PS2"),
	COLOR_SUBPROMPT)))
	{
		ft_strdel(&pmt_prefix);
		return (0);
	}
	if (!(*lexer->src = ft_strclnjoin(*lexer->src, new)))
	{
		ft_printf("erreur malloc complete_flag\n");
		exit(1);
	}
	lexer->curr = *lexer->src + pos;
	ft_strdel(&pmt_prefix);
	return (1);
}
