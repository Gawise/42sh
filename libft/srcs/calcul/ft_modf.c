/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_modf.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guaubret <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 20:40:16 by guaubret          #+#    #+#             */
/*   Updated: 2019/08/19 20:40:18 by guaubret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static double	get_val(double val, double *integral, t_modf u, int e)
{
	*integral = val;
	if (e == 0x400 && (u.i << 12) != 0)
		return (val);
	u.i &= (1ULL << 63);
	return (u.f);
}

double			ft_modf(double val, double *integral)
{
	t_modf				u;
	long long unsigned	mask;
	int					e;

	u.f = val;
	e = (int)(u.i >> 52 & 0x7ff) - 0x3ff;
	if (e >= 52)
		return (get_val(val, integral, u, e));
	if (e < 0)
	{
		u.i &= (1ULL << 63);
		*integral = u.f;
		return (val);
	}
	mask = (-1ULL >> 12 >> e);
	if ((u.i & mask) == 0)
	{
		*integral = val;
		u.i &= 1ULL << 63;
		return (u.f);
	}
	u.i &= ~mask;
	*integral = u.f;
	return (val - u.f);
}
