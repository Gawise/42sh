/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   equ.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/10 14:46:13 by user42            #+#    #+#             */
/*   Updated: 2020/07/10 14:46:13 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_printf.h"
#include "lexer.h"

int	l_delim_equ(t_lexer *lexer, char c)
{
	if (!ft_isname(lexer->buffer))
		return (l_build_word(lexer, c));
	l_delim_token(lexer, c);
	lexer->curr_token->type = ASSIGNMENT_WORD;
	l_build_word(lexer, c);
	return (1);
}
