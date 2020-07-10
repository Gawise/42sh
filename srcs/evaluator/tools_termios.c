/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools_termios.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/10 14:46:09 by user42            #+#    #+#             */
/*   Updated: 2020/07/10 17:44:17 by hmerieux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "exec.h"
#include "sh.h"

uint8_t	set_termios(int32_t mode, struct termios *term)
{
	if (tcsetattr(STDIN_FILENO, mode, term) == -1)
		return (FAILURE);
	return (SUCCESS);
}

void	term_create_eval(struct termios *origin, struct termios *eval)
{
	ft_memcpy(eval, origin, sizeof(struct termios));
	eval->c_lflag &= ~TOSTOP;
}
