/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ftoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guaubret <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/20 19:46:05 by guaubret          #+#    #+#             */
/*   Updated: 2019/08/19 20:39:08 by guaubret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_ftoa(double val, int prec, char res[CVTBUFSZ])
{
	char	tmp[CVTBUFSZ];
	int		decpt;
	int		i;

	if (prec >= CVTBUFSZ - 1)
		prec = CVTBUFSZ - 2;
	if (prec < 0)
		prec = 6;
	i = 0;
	if (val < 0)
	{
		res[i++] = '-';
		val = -val;
	}
	if (val < 1)
		res[i++] = '0';
	ft_fcvt(val, prec, &decpt, tmp);
	ft_strncpy(res + i, tmp, decpt);
	res[decpt + i] = '.';
	ft_strcpy(res + i + decpt + 1, tmp + decpt);
}
