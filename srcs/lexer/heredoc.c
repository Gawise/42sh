#include "libft.h"
#include "ft_printf.h"
#include "lexer.h"

int		l_create_flag_queue(t_lexer *lexer)
{
	if (!ft_lstpush(&lexer->flag_queue, lexer->curr_flag, sizeof(t_lexer_flag)))
	{
		ft_printf("erreur malloc create_flag_queue\n");
		exit(1);
	}
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

void	del_here_queue(void *data, size_t size)
{
	t_here_queue	*queue;

	(void)size;
	queue = (t_here_queue *)data;
	ft_strdel(&queue->delim);
	ft_memdel((void**)&queue);
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

int	l_hd_body_flush(t_lexer *lexer, char c)
{
	if (ft_strnequ(lexer->buffer, lexer->curr_here->delim, ft_strlen(lexer->curr_here->delim)))
	{
		ft_bzero(lexer->buffer, L_BUFF_SIZE);
		lexer->buff_i = 0;
		ft_lstdelhead(&lexer->here_queue, &del_here_queue);
		if (lexer->here_queue)
		{
			lexer->curr_here = lexer->here_queue->data;
			lexer->curr_token = lexer->curr_here->token;
		}
		if (!lexer->curr_here)
			lexer->state = S_TK_START;
	}
	else
	{
		l_buffer_add(lexer, c);
		l_buffer_flush(lexer);
	}
	return (1);
}
