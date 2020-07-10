/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/10 14:46:15 by user42            #+#    #+#             */
/*   Updated: 2020/07/10 18:02:48 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_printf.h"
#include "lexer.h"

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

int	l_delim_redir(t_lexer *lexer, char c)
{
	l_delim_token(lexer, c);
	lexer->state = S_TK_START;
	l_build_redir(lexer, c);
	return (1);
}

int	l_add_hyphen_redir(t_lexer *lexer, char c)
{
	if (ft_strnequ("<<", lexer->buffer, 2))
	{
		l_buffer_add(lexer, c);
		lexer->curr_token->type = DLESSDASH;
	}
	return (1);
}
