#include "libft.h"
#include "ft_printf.h"
#include "lexer.h"

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

void	del_flag_queue(void *data, size_t size)
{
	(void)size;
	free(data);
}
