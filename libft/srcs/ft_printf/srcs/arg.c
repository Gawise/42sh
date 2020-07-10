/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/10 15:09:44 by user42            #+#    #+#             */
/*   Updated: 2020/07/10 15:09:44 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void		read_arg(t_printf *data)
{
	if (*(data->fmt) == '{')
		conv_color(data);
	else
	{
		data->flags = 0;
		data->width = 0;
		data->prec = 0;
		data->conv = 0;
		data->err_ptr = data->fmt;
		data->fmt++;
		read_flags(data);
		read_width(data);
		read_prec(data);
		read_length_mod(data);
		read_conv_spec(data);
	}
}
