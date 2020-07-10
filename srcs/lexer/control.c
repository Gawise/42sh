/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   control.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/10 14:46:12 by user42            #+#    #+#             */
/*   Updated: 2020/07/10 14:46:12 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_printf.h"
#include "lexer.h"

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

int	l_delim_control(t_lexer *lexer, char c)
{
	l_delim_token(lexer, c);
	lexer->state = S_TK_START;
	l_build_control(lexer, c);
	return (1);
}
