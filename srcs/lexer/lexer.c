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

void	print_flag_queue(t_list *elem)
{
	t_lexer_flag	*flag;

	flag = (t_lexer_flag *)elem->data;
	ft_printf("%d\n", *flag);
}

char	*escape_nl(char *src)
{
	char	*str;
	int	cnt;
	int	i;
	int	j;

	cnt = 0;
	i = 0;
	if (!src)
		return (NULL);
	while (src[i])
	{
		if (src[i] == '\n')
			cnt++;
		i++;
	}
	i = 0;
	j = 0;
	str = ft_strnew(ft_strlen(src) + cnt);
	while (src[i])
	{
		if (src[i] == '\n')
		{
			str[j++] = '\\';
			str[j++] = 'n';
			i++;
		}
		else
			str[j++] = src[i++];
	}
	str[j] = '\0';
	return (str);
}

void	print_debug(t_list *elem)
{
	t_token *token = elem->data;
	char	*str;
	if (token->type == TOKEN)
		str = ft_strdup("TOKEN");
	if (token->type == WORD)
		str = ft_strdup("WORD");
	if (token->type == ASSIGNMENT_WORD)
		str = ft_strdup("ASSIGNMENT_WORD");
	if (token->type == NEWLINE)
	{
		ft_printf("str=%10.10s\ttype= %15s\n", "\\n", "NEWLINE");
		return ;
	}
	if (token->type == IO_NUMBER)
		str = ft_strdup("IO_NUMBER");
	if (token->type == AND_IF)
		str = ft_strdup("AND_IF");
	if (token->type == AMP)
		str = ft_strdup("AMP");
	if (token->type == OR_IF)
		str = ft_strdup("OR_IF");
	if (token->type == PIPE)
		str = ft_strdup("PIPE");
	if (token->type == SEMI)
		str = ft_strdup("SEMI");
	if (token->type == LESS)
		str = ft_strdup("LESS");
	if (token->type == DLESS)
		str = ft_strdup("DLESS");
	if (token->type == GREAT)
		str = ft_strdup("GREAT");
	if (token->type == DGREAT)
		str = ft_strdup("DGREAT");
	if (token->type == LESSAND)
		str = ft_strdup("LESSAND");
	if (token->type == GREATAND)
		str = ft_strdup("GREATAND");
	if (token->type == DLESSDASH)
		str = ft_strdup("DLESSDASH");
	ft_printf("str=%30s\ttype= %15s\t%p\n", escape_nl(token->str), str, token);
}

int	lex_err(t_lexer *lexer, char c)
{
	(void)lexer;
	(void)c;
	ft_printf("lexing error\n");
	exit(1);
	return (1);
}

int	l_set_ctrl_type(t_lexer *lexer, char c)
{
	(void)c;
	if (ft_strnequ(lexer->curr_token->str, "&&", 2))
		lexer->curr_token->type = AND_IF;
	else if (ft_strnequ(lexer->curr_token->str, "&", 1))
		lexer->curr_token->type = AMP;
	else if (ft_strnequ(lexer->curr_token->str, "||", 2))
		lexer->curr_token->type = OR_IF;
	else if (ft_strnequ(lexer->curr_token->str, "|", 1))
		lexer->curr_token->type = PIPE;
	return (1);
}

int	l_set_redir_type(t_lexer *lexer, char c)
{
	(void)c;
	if (ft_strnequ(lexer->curr_token->str, "<<-", 3))
		lexer->curr_token->type = DLESSDASH;
	else if (ft_strnequ(lexer->curr_token->str, "<<", 2))
		lexer->curr_token->type = DLESS;
	else if (ft_strnequ(lexer->curr_token->str, ">>", 2))
		lexer->curr_token->type = DGREAT;
	else if (ft_strnequ(lexer->curr_token->str, "<&", 2))
		lexer->curr_token->type = LESSAND;
	else if (ft_strnequ(lexer->curr_token->str, ">&", 2))
		lexer->curr_token->type = GREATAND;
	else if (ft_strnequ(lexer->curr_token->str, "<", 1))
		lexer->curr_token->type = LESS;
	else if (ft_strnequ(lexer->curr_token->str, ">", 1))
		lexer->curr_token->type = GREAT;
	return (1);
}

int	l_set_word_type(t_lexer *lexer, char c)
{
	int	i;

	i = -1;
	while (lexer->curr_token->str[++i])
		if (!ft_strchr("0123456789", lexer->curr_token->str[i])) // is_digit
		{
			lexer->curr_token->type = WORD;
			return (1);
		}
	if (ft_strchr("<>", c))
		lexer->curr_token->type = IO_NUMBER;
	else
		lexer->curr_token->type = WORD;
	return (1);
}

int	l_set_token_type(t_lexer *lexer, char c)
{
	if (lexer->state == S_TK_START)
		return (0);
	else if (lexer->state == S_HD_BODY)
		l_set_word_type(lexer, c);
	else if (lexer->state == S_AMP_PIPE)
		l_set_ctrl_type(lexer, c);
	else if (lexer->state == S_TK_REDIR)
		l_set_redir_type(lexer, c);
	else if (lexer->state == S_TK_WORD)
		l_set_word_type(lexer, c);
	else if (lexer->state == S_IO_NUMBER)
		l_set_word_type(lexer, c);
	else
		lex_err(lexer, c);
	return (1);
}

void		init_token(t_token *token)
{
	token->str = NULL;
	token->type = 0;
	token->len = 0;
}

int		l_create_flag_queue(t_lexer *lexer)
{
	if (!ft_lstpush(&lexer->flag_queue, lexer->curr_flag, sizeof(t_lexer_flag)))
	{
		ft_printf("erreur malloc create_flag_queue\n");
		exit(1);
	}
	return (1);
}

int		l_create_token(t_lexer *lexer)
{
	if (!(lexer->curr_token = (t_token *)ft_memalloc(sizeof(t_token))))
	{
		ft_printf("erreur malloc create_token 1\n");
		exit(EXIT_FAILURE);
	}
	init_token(lexer->curr_token);
	if (!ft_lstpush(&lexer->token_lst, lexer->curr_token, sizeof(t_token)))
	{
		ft_printf("erreur malloc create_token 2\n");
		exit(EXIT_FAILURE);
	}
	//ft_printf("creation token %p\n", lexer->curr_token);
	return (1);
}

int		l_create_here_queue(t_lexer *lexer)
{
	if (!(lexer->curr_here = (t_here_queue *)ft_memalloc(sizeof(t_here_queue))))
	{
		ft_printf("erreur malloc create_here_queue 1\n");
		exit(EXIT_FAILURE);
	}
	if (!ft_lstpush(&lexer->here_queue, lexer->curr_here, sizeof(t_here_queue)))
	{
		ft_printf("erreur malloc create_here_queue 2\n");
		exit(EXIT_FAILURE);
	}
	return (1);
}

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

int	l_is_quoting(t_lexer *lexer)
{
	if (!l_get_flag(lexer, F_DQUOTE)
	&& !l_get_flag(lexer, F_SQUOTE)
	&& !l_get_flag(lexer, F_BSLASH))
		return (0);
	return (1);
}

char	l_get_char(t_lexer *lexer)
{
	if (!*(lexer->curr))
		return (0);
	return (*lexer->curr++);
}

void		l_buffer_flush(t_lexer *lexer)
{
	t_token		*token;

	token = lexer->curr_token;
	if (!token->str)
	{
		if (!(token->str = ft_strndup(lexer->buffer, lexer->buff_i)))
		{
			ft_printf("erreur malloc buffer_flush 1\n");
			exit(EXIT_FAILURE);
		}
	}
	else if (!(token->str = ft_strnlclnjoin(token->str, lexer->buffer, lexer->buff_i)))
	{
		ft_printf("erreur malloc buffer flush 2\n");
		exit(EXIT_FAILURE);
	}
	//ft_printf("\n");
	ft_bzero(lexer->buffer, L_BUFF_SIZE);
	lexer->buff_i = 0;
}

int		l_buffer_add(t_lexer *lexer, char c)
{
	if (lexer->buff_i >= L_BUFF_SIZE)
		l_buffer_flush(lexer);
	lexer->buffer[lexer->buff_i] = c;
	lexer->buff_i++;
	return (1);
}

void	del_here_queue(void *data, size_t size)
{
	t_here_queue	*queue;

	(void)size;
	queue = (t_here_queue *)data;
	ft_strdel(&queue->delim);
	ft_memdel((void**)&queue);
}

void	del_flag_queue(void *data, size_t size)
{
	(void)size;
	free(data);
}

int	l_exp_word(t_lexer *lexer, char c)
{
	l_buffer_add(lexer, c);
	return (1);
}

void	l_flush_delim(t_lexer *lexer)
{
	if (!(lexer->curr_here->delim = ft_strndup(lexer->buffer, lexer->buff_i)))
	{
		ft_printf("erreur malloc flush_delim 1\n");
		exit(1);
	}
	ft_bzero(lexer->buffer, L_BUFF_SIZE);
	lexer->buff_i = 0;
}

int	l_delim_token(t_lexer *lexer, char c)
{
	if (l_get_flag(lexer, F_HD_DELIM))
	{
		if (!(l_create_token(lexer)))
			return (0);
		lexer->curr_here->token = lexer->curr_token;
		lexer->curr_token->type = WORD;
		l_unset_flag(lexer, F_HD_DELIM);
		l_flush_delim(lexer);
		lexer->state = S_TK_START;
		return (1);
	}
	l_buffer_flush(lexer);
	if (lexer->curr_token->type == TOKEN)
		l_set_token_type(lexer, c);
	if (lexer->curr_token->type == DLESS
	|| lexer->curr_token->type == DLESSDASH)
	{
		l_set_flag(lexer, F_HD_DELIM);
		l_create_here_queue(lexer);
		//ft_printf("creation here_queue %p\n", lexer->curr_here);
		l_set_flag(lexer, F_HEREDOC);
	}
	lexer->state = S_TK_START;
	//ft_printf("\n-------------------------------\n");
	//ft_lstiter(lexer->token_lst, print_debug);
	//ft_printf("-------------------------------\n\n");
	return (1);
}

int	l_hd_body_flush(t_lexer *lexer, char c)
{
	//ft_printf("hd_body_flush\n");
	if (ft_strnequ(lexer->buffer, lexer->curr_here->delim, ft_strlen(lexer->curr_here->delim)))
	{
		//ft_printf("hd_body_flush delim ok\n");
		ft_bzero(lexer->buffer, L_BUFF_SIZE);
		lexer->buff_i = 0;
		//ft_printf("heredoc actuel %p\n", lexer->curr_here);
		ft_lstdelhead(&lexer->here_queue, &del_here_queue);
		if (lexer->here_queue)
		{
			lexer->curr_here = lexer->here_queue->data;
			lexer->curr_token = lexer->curr_here->token;
			//ft_printf("nouveau heredoc %p\n", lexer->curr_here);
		}
		if (!lexer->curr_here)
			lexer->state = S_TK_START;
	}
	else
	{
		//ft_printf("flush body \'%s\' dans %p\n", lexer->buffer, lexer->curr_token);
		l_buffer_add(lexer, c);
		l_buffer_flush(lexer);
		//ft_printf("%s\n", lexer->curr_token->str);
	}
	//ft_printf("\n-------------------------------\n");
	//ft_lstiter(lexer->token_lst, print_debug);
	//ft_printf("-------------------------------\n\n");
	return (1);
}

int	l_do_nothing(t_lexer *lexer, char c)
{
	(void)lexer;
	(void)c;
	return (1);
}

int	l_build_exp(t_lexer *lexer, char c)
{
	if (lexer->state == S_TK_START && !l_create_token(lexer))
		return (0);
	l_buffer_add(lexer, c);
	lexer->state = S_EXP;
	return (1);
}

t_lexer_flag	l_get_last_flag(t_lexer *lexer)
{
	t_list	*flag;

	flag = ft_lstgettail(&lexer->flag_queue);
	if (flag && flag->data)
	{
		ft_printf("----------flag_queue---------\n");
		ft_lstiter(lexer->flag_queue, print_flag_queue);
		ft_printf("-----------------------------\n\n");
		return (*((t_lexer_flag *)flag->data));
	}
	else
		return (-1);
}

int	l_add_flag(t_lexer *lexer, char c)
{
	if (!(lexer->curr_flag = (t_lexer_flag *)ft_memalloc(sizeof(t_lexer_flag))))
		return (0);
	if (c == '\'')
		*lexer->curr_flag = F_SQUOTE;
	else if (c == '\"')
		*lexer->curr_flag = F_DQUOTE;
	else if (c == '{')
		*lexer->curr_flag = F_BRACKEXP;
	l_create_flag_queue(lexer);
	return (1);
}

int	l_bslash_add(t_lexer *lexer, char c)
{
	if (c != '\n')
		l_buffer_add(lexer, c);
	l_unset_flag(lexer, F_BSLASH);
	return (1);
}

int	l_dquote_add(t_lexer *lexer, char c)
{
	if (c == '$')
		l_build_exp(lexer, c);
	else if (c == '\\')
		l_set_flag(lexer, F_BSLASH);
	else
		l_buffer_add(lexer, c);
	return (1);
}

int	l_flag_state_add(t_lexer *lexer, char c)
{
	t_lexer_flag	flag;

	flag = l_get_last_flag(lexer);
	if (flag == F_DQUOTE)
	{
		if (c == '$')
			l_build_exp(lexer, c);
		else if (c == '\\')
			l_bslash_add(lexer, c);
		else
			l_buffer_add(lexer, c);
	}
	else if (flag == F_BRACKEXP && c == '}')
	{
		l_buffer_add(lexer, c);
		lexer->state = S_TK_WORD;
		ft_lstdeltail(&lexer->flag_queue, del_flag_queue);
	}
	else
		l_buffer_add(lexer, c);
	return (1);
}

int	l_exp_brack(t_lexer *lexer, char c)
{
	l_buffer_add(lexer, c);
	if (c == '{')
	{
		l_add_flag(lexer, c);
		lexer->state = S_FLAG;
	}
	else
	{
		ft_lstdeltail(&lexer->flag_queue, del_flag_queue);
		if (l_get_last_flag(lexer) > 0)
			lexer->state = S_FLAG;
		else
			lexer->state = S_TK_WORD;
	}
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

int	l_build_word(t_lexer *lexer, char c)
{
	if (lexer->state == S_TK_START && !l_get_flag(lexer, F_HD_DELIM)
	&& !l_create_token(lexer))
		return (0);
	l_buffer_add(lexer, c);
	lexer->state = S_TK_WORD;
	return (1);
}

int	l_build_digit(t_lexer *lexer, char c)
{
	l_buffer_add(lexer, c);
	if (lexer->state == S_TK_START && !l_create_token(lexer))
		return (0);
	lexer->state = S_IO_NUMBER;
	return (1);
}

int	l_is_duplicable_redir(t_lexer *lexer)
{
	if (!ft_strnequ(lexer->buffer, ">>", 2)
	&& ft_strnequ(lexer->buffer, ">", 1))
		return (1);
	if (!ft_strnequ(lexer->buffer, "<<", 2)
	&& ft_strnequ(lexer->buffer, "<", 1))
		return (1);
	return (0);
}

int	l_delim_control(t_lexer *lexer, char c);

int	l_build_redir(t_lexer *lexer, char c)
{
	if (lexer->state == S_TK_REDIR)
	{
		if (c == '&' && !l_is_duplicable_redir(lexer)
		&& !l_delim_control(lexer, c))
			return (0);
		l_buffer_add(lexer, c);
		if (!ft_strnequ("<<", lexer->buffer, 2))
		{
			l_delim_token(lexer, c);
			lexer->state = S_TK_START;
		}
	}
	else
	{
		l_buffer_add(lexer, c);
		lexer->state = S_TK_REDIR;
		if (!l_create_token(lexer))
			return (0);
	}
	return (1);
}

int	l_build_control(t_lexer *lexer, char c)
{
	if (c == ';')
	{
		l_buffer_add(lexer, c);
		if (!l_create_token(lexer))
			return (0);
		lexer->curr_token->type = SEMI;
		l_buffer_flush(lexer);
		lexer->state = S_TK_START;
	}
	else
	{
		if (lexer->state == S_TK_START && !l_create_token(lexer))
			return (0);
		if (ft_strchr("&|", c))
		{
			lexer->state = S_AMP_PIPE;
			l_buffer_add(lexer, c);
		}
	}
	return (1);
}

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
	return (1);
}

int	l_build_newline(t_lexer *lexer, char c)
{
	if (l_get_flag(lexer, F_HEREDOC))
	{
		lexer->state = S_HD_BODY;
		lexer->curr_here = lexer->here_queue->data;
		lexer->curr_token = lexer->curr_here->token;
		//ft_printf("coucou token %p dans here %p\n", lexer->curr_here->token, lexer->curr_here);
		return (1);
	}
	else if (!l_create_token(lexer))
		return (0);
	l_buffer_add(lexer, c);
	lexer->curr_token->type = NEWLINE;
	l_buffer_flush(lexer);
	return (1);
}

int	l_delim_flag(t_lexer *lexer, char c)
{
	t_lexer_flag	flag;

	flag = l_get_last_flag(lexer);
	if (c == '\'' && flag != F_SQUOTE && flag != F_BSLASH)
		l_add_flag(lexer, c);
	else if (c == '\'' && flag == F_SQUOTE)
		ft_lstdeltail(&lexer->flag_queue, del_flag_queue);
	else if (c == '\"' && flag == F_DQUOTE)
		ft_lstdeltail(&lexer->flag_queue, del_flag_queue);
	else if (c == '\\' && flag == F_BSLASH)
		ft_lstdeltail(&lexer->flag_queue, del_flag_queue);
	else if (c == '}' && flag == F_BRACKEXP)
		ft_lstdeltail(&lexer->flag_queue, del_flag_queue);
	l_buffer_add(lexer, c);
	if (!l_get_last_flag(lexer))
		lexer->state = S_TK_WORD;
	return (1);
}

int	l_delim_equ(t_lexer *lexer, char c)
{
	l_delim_token(lexer, c);
	lexer->curr_token->type = ASSIGNMENT_WORD;
	l_build_word(lexer, c);
	return (1);
}

int	l_delim_word(t_lexer *lexer, char c)
{
	l_delim_token(lexer, c);
	l_build_word(lexer, c);
	return (1);
}

int	l_delim_exp(t_lexer *lexer, char c)
{
	l_delim_token(lexer, c);
	l_build_exp(lexer, c);
	return (1);
}

int	l_delim_digit(t_lexer *lexer, char c)
{
	l_delim_token(lexer, c);
	l_build_digit(lexer, c);
	return (1);
}

int	l_delim_newline(t_lexer *lexer, char c)
{
	l_delim_token(lexer, c);
	l_build_newline(lexer, c);
	return (1);
}

int	l_delim_redir(t_lexer *lexer, char c)
{
	l_delim_token(lexer, c);
	lexer->state = S_TK_START;
	l_build_redir(lexer, c);
	return (1);
}

int	l_delim_control(t_lexer *lexer, char c)
{
	l_delim_token(lexer, c);
	l_build_control(lexer, c);
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

void	init_start_state(int (*token_builder[8][11])(t_lexer *, char))
{
	token_builder[S_TK_START][0] = l_build_inhib;
	token_builder[S_TK_START][1] = l_build_control;
	token_builder[S_TK_START][2] = l_build_redir;
	token_builder[S_TK_START][3] = l_build_newline;
	token_builder[S_TK_START][4] = l_build_digit;
	token_builder[S_TK_START][5] = l_build_exp;
	token_builder[S_TK_START][6] = l_do_nothing;
	token_builder[S_TK_START][7] = l_do_nothing;
	token_builder[S_TK_START][8] = l_build_word;
	token_builder[S_TK_START][9] = l_build_word;
	token_builder[S_TK_START][10] = l_build_word;
}

void	init_heredoc_body_state(int (*token_builder[8][11])(t_lexer *, char))
{
	token_builder[S_HD_BODY][0] = l_buffer_add;
	token_builder[S_HD_BODY][1] = l_buffer_add;
	token_builder[S_HD_BODY][2] = l_buffer_add;
	token_builder[S_HD_BODY][3] = l_hd_body_flush;
	token_builder[S_HD_BODY][4] = l_buffer_add;
	token_builder[S_HD_BODY][5] = l_buffer_add;
	token_builder[S_HD_BODY][6] = l_buffer_add;
	token_builder[S_HD_BODY][7] = l_do_nothing;
	token_builder[S_HD_BODY][8] = l_buffer_add;
	token_builder[S_HD_BODY][9] = l_buffer_add;
	token_builder[S_HD_BODY][10] = l_buffer_add;
}

void	init_amp_pipe_state(int (*token_builder[8][11])(t_lexer *, char))
{
	token_builder[S_AMP_PIPE][0] = l_delim_inhib;
	token_builder[S_AMP_PIPE][1] = l_build_control;
	token_builder[S_AMP_PIPE][2] = l_delim_redir;
	token_builder[S_AMP_PIPE][3] = l_delim_newline;
	token_builder[S_AMP_PIPE][4] = l_delim_digit;
	token_builder[S_AMP_PIPE][5] = l_delim_exp;
	token_builder[S_AMP_PIPE][6] = l_delim_token;
	token_builder[S_AMP_PIPE][7] = l_do_nothing;
	token_builder[S_AMP_PIPE][8] = l_delim_word;
	token_builder[S_AMP_PIPE][9] = l_delim_word;
	token_builder[S_AMP_PIPE][10] = l_delim_word;
}

void	init_redir_state(int (*token_builder[8][11])(t_lexer *, char))
{
	token_builder[S_TK_REDIR][0] = l_delim_inhib;
	token_builder[S_TK_REDIR][1] = l_build_redir;
	token_builder[S_TK_REDIR][2] = l_build_redir;
	token_builder[S_TK_REDIR][3] = l_delim_newline;
	token_builder[S_TK_REDIR][4] = l_delim_digit;
	token_builder[S_TK_REDIR][5] = l_delim_exp;
	token_builder[S_TK_REDIR][6] = l_delim_token;
	token_builder[S_TK_REDIR][7] = l_do_nothing;
	token_builder[S_TK_REDIR][8] = l_delim_word;
	token_builder[S_TK_REDIR][9] = l_delim_word;
	token_builder[S_TK_REDIR][10] = l_delim_word;
}

void	init_exp_state(int (*token_builder[8][11])(t_lexer *, char))
{
	token_builder[S_EXP][0] = l_build_inhib;
	token_builder[S_EXP][1] = l_delim_control;
	token_builder[S_EXP][2] = l_delim_redir;
	token_builder[S_EXP][3] = l_delim_newline;
	token_builder[S_EXP][4] = l_buffer_add;
	token_builder[S_EXP][5] = l_build_exp;
	token_builder[S_EXP][6] = l_buffer_add;
	token_builder[S_EXP][7] = l_delim_token;
	token_builder[S_EXP][8] = l_exp_brack;
	token_builder[S_EXP][9] = l_exp_word;
	token_builder[S_EXP][10] = l_exp_word;
}

void	init_word_state(int (*token_builder[8][11])(t_lexer *, char))
{
	token_builder[S_TK_WORD][0] = l_build_inhib;
	token_builder[S_TK_WORD][1] = l_delim_control;
	token_builder[S_TK_WORD][2] = l_delim_redir;
	token_builder[S_TK_WORD][3] = l_delim_newline;
	token_builder[S_TK_WORD][4] = l_buffer_add;
	token_builder[S_TK_WORD][5] = l_build_exp;
	token_builder[S_TK_WORD][6] = l_delim_token;
	token_builder[S_TK_WORD][7] = l_delim_token;
	token_builder[S_TK_WORD][8] = l_buffer_add;
	token_builder[S_TK_WORD][9] = l_delim_equ;
	token_builder[S_TK_WORD][10] = l_buffer_add;
}

void	init_io_number_state(int (*token_builder[8][11])(t_lexer *, char))
{
	token_builder[S_IO_NUMBER][0] = l_build_inhib;
	token_builder[S_IO_NUMBER][1] = l_delim_control;
	token_builder[S_IO_NUMBER][2] = l_delim_redir;
	token_builder[S_IO_NUMBER][3] = l_delim_newline;
	token_builder[S_IO_NUMBER][4] = l_buffer_add;
	token_builder[S_IO_NUMBER][5] = l_delim_exp;
	token_builder[S_IO_NUMBER][6] = l_delim_token;
	token_builder[S_IO_NUMBER][7] = l_delim_token;
	token_builder[S_IO_NUMBER][8] = l_buffer_add;
	token_builder[S_IO_NUMBER][9] = l_buffer_add;
	token_builder[S_IO_NUMBER][10] = l_buffer_add;
}

void	init_inhib_state(int (*token_builder[8][11])(t_lexer *, char))
{
	token_builder[S_FLAG][0] = l_delim_flag;
	token_builder[S_FLAG][1] = l_flag_state_add;
	token_builder[S_FLAG][2] = l_flag_state_add;
	token_builder[S_FLAG][3] = l_flag_state_add;
	token_builder[S_FLAG][4] = l_flag_state_add;
	token_builder[S_FLAG][5] = l_flag_state_add;
	token_builder[S_FLAG][6] = l_flag_state_add;
	token_builder[S_FLAG][7] = l_flag_state_add;
	token_builder[S_FLAG][8] = l_delim_flag;
	token_builder[S_FLAG][9] = l_flag_state_add;
	token_builder[S_FLAG][10] = l_flag_state_add;
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
	init_inhib_state(token_builder);
}

char	*get_state_str(t_lexer *lexer)
{
	if (lexer->state == 0)
		return (ft_strdup("S_TK_START"));
	else if (lexer->state == 1)
		return (ft_strdup("S_HD_BODY"));
	else if (lexer->state == 2)
		return (ft_strdup("S_AMP_PIPE"));
	else if (lexer->state == 3)
		return (ft_strdup("S_TK_REDIR"));
	else if (lexer->state == 4)
		return (ft_strdup("S_EXP"));
	else if (lexer->state == 5)
		return (ft_strdup("S_TK_WORD"));
	else if (lexer->state == 6)
		return (ft_strdup("S_IO_NUMBER"));
	else if (lexer->state == 7)
		return (ft_strdup("S_FLAG"));
	return (NULL);
}

int	ft_lexer(char *str, t_lexer *lexer)
{
	char	c;
	int	(*token_builder[8][11])(t_lexer *, char);

	lexer->src = str;
	lexer->curr = str;
	init_lexer_states(token_builder);
	while ((c = l_get_char(lexer)))
	{
		if (!token_builder[lexer->state][l_get_char_type(c)](lexer, c))
			lex_err(lexer, c);
		/*if (c == '\n')
			ft_printf("->\t%s\t%s\n\n", "\\n", get_state_str(lexer));
		else
			ft_printf("->\t%c\t%s\n", c, get_state_str(lexer));
		*/
	}
	if (!token_builder[lexer->state][l_get_char_type(c)](lexer, c))
		lex_err(lexer, c);
	return (1);
}
