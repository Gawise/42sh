/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hdbody.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/10 14:46:16 by user42            #+#    #+#             */
/*   Updated: 2020/07/10 14:46:16 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_printf.h"
#include "lexer.h"

void	init_heredoc_body_state(int (*token_builder[9][12])(t_lexer *, char))
{
	token_builder[S_HD_BODY][C_INHIBITOR] = l_buffer_add;
	token_builder[S_HD_BODY][C_CONTROL] = l_buffer_add;
	token_builder[S_HD_BODY][C_REDIR] = l_buffer_add;
	token_builder[S_HD_BODY][C_NEWLINE] = l_hd_body_flush;
	token_builder[S_HD_BODY][C_DIGIT] = l_buffer_add;
	token_builder[S_HD_BODY][C_EXP] = l_buffer_add;
	token_builder[S_HD_BODY][C_BLANK] = l_buffer_add;
	token_builder[S_HD_BODY][C_EOI] = l_complete_str;
	token_builder[S_HD_BODY][C_BRACK] = l_buffer_add;
	token_builder[S_HD_BODY][C_EQU] = l_buffer_add;
	token_builder[S_HD_BODY][C_HASH] = l_buffer_add;
	token_builder[S_HD_BODY][C_OTHER] = l_buffer_add;
}
