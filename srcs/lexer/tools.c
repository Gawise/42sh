/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/10 14:46:17 by user42            #+#    #+#             */
/*   Updated: 2020/07/10 14:46:17 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

void	del_here_queue(void *data, size_t size)
{
	t_here_queue	*queue;

	(void)size;
	queue = (t_here_queue *)data;
	ft_strdel(&queue->delim);
	ft_memdel((void**)&queue);
}
