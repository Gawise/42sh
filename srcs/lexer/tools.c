#include "libft.h"
#include "ft_printf.h"
#include "lexer.h"
#include "var.h"
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

int		l_get_flag(t_lexer *lexer, t_lexer_flag flag)
{
	return (lexer->flags & (1 << flag));
}

int		l_do_nothing(t_lexer *lexer, char c)
{
	(void)lexer;
	(void)c;
	return (1);
}

static char	*l_get_prompt_prefix(t_lexer *lexer)
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

int		l_complete_str(t_lexer *lexer,  char c)
{
	size_t	pos;
	char	*new;
	char	*pmt_prefix;

	(void)c;
	new = NULL;
	pos = lexer->curr - *lexer->src;
	if (!(pmt_prefix = l_get_prompt_prefix(lexer))
	|| ft_dprintf(0, "%s", pmt_prefix) == -1
	|| !(new = ft_prompt(find_var_value(cfg_shell()->intern, "PS2"), COLOR_SUBPROMPT)))
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
