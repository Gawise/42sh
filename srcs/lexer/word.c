/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/10 14:46:18 by user42            #+#    #+#             */
/*   Updated: 2020/07/10 18:02:40 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_printf.h"
#include "lexer.h"

int	l_build_word(t_lexer *lexer, char c)
{
	if (lexer->state == S_TK_START && !l_get_flag(lexer, F_HD_DELIM)
	&& !l_create_token(lexer))
		return (0);
	l_buffer_add(lexer, c);
	lexer->state = S_TK_WORD;
	return (1);
}

int	l_delim_word(t_lexer *lexer, char c)
{
	if (lexer->state == S_TK_REDIR && c == '-')
		return (l_add_hyphen_redir(lexer, c));
	l_delim_token(lexer, c);
	l_build_word(lexer, c);
	return (1);
}
