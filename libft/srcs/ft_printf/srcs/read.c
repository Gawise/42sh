/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guaubret <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/19 19:20:57 by guaubret          #+#    #+#             */
/*   Updated: 2019/08/19 19:32:48 by guaubret         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void		read_flags(t_printf *data)
{
	while ((data->f_index = ft_strichr("#0- +", *(data->fmt))) > -1)
	{
		data->flags |= (1 << data->f_index);
		(data->fmt)++;
	}
	if (CH_MINUS(data->flags))
		data->flags &= ~F_ZERO;
	if (CH_PLUS(data->flags))
		data->flags &= ~F_SPACE;
}

void		read_width(t_printf *data)
{
	if (ft_isdigit(*data->fmt))
	{
		data->width = ft_atoi(data->fmt);
		while (ft_isdigit(*(data->fmt)))
			(data->fmt)++;
	}
	else if (*(data->fmt) == '*')
	{
		data->width = va_arg(data->ap, int);
		if (data->width < 0)
		{
			data->width = -data->width;
			data->flags |= F_MINUS;
		}
		(data->fmt)++;
	}
}

void		read_prec(t_printf *data)
{
	if (*(data->fmt) == '.')
	{
		(data->fmt)++;
		if (*(data->fmt) == '*')
		{
			data->prec = va_arg(data->ap, int);
			(data->fmt)++;
		}
		else
		{
			if (ft_isdigit(*(data->fmt)))
				data->prec = ft_atoi(data->fmt);
			while (ft_isdigit(*(data->fmt)))
				(data->fmt)++;
		}
		if (data->prec >= 0)
			data->flags |= F_PGIVEN;
		data->prec = (data->prec < 0) ? 0 : data->prec;
	}
}

void		read_length_mod(t_printf *data)
{
	if (*(data->fmt) == 'l')
	{
		data->flags |= (*(data->fmt + 1) == 'l') ? F_LL : F_L;
		(data->fmt)++;
	}
	else if (*(data->fmt) == 'h')
	{
		data->flags |= (*(data->fmt + 1) == 'h') ? F_HH : F_H;
		(data->fmt)++;
	}
	else if (*(data->fmt) == 'L')
		data->flags |= F_LL;
	else if (*(data->fmt) == 'z')
		data->flags |= F_Z;
	else if (*(data->fmt) == 'j')
		data->flags |= F_J;
	if (ft_strchr("lhLzj", *(data->fmt)))
		(data->fmt)++;
	if (ft_strchr("DOUFBCSX", *data->fmt))
		data->flags |= (*data->fmt == 'X') ? F_UPX : F_L;
}

void		read_conv_spec(t_printf *data)
{
	int				i;
	static void		(*disp[11])(t_printf *);

	if (!data->disp_set)
		init_dispatcher(data, disp);
	data->conv = *(data->fmt);
	if (ft_strchr("idDoOuUxXbB", *(data->fmt)) && CH_PGIVEN(data->flags))
		data->flags &= ~F_ZERO;
	if (*(data->fmt) == 'i')
		disp[0](data);
	else if ((i = ft_strichr("DOUFBCSX", *(data->fmt))) > -1)
		disp[i](data);
	else if ((i = ft_strichr("doufbcsxp%m{", *(data->fmt))) > -1)
		disp[i](data);
	else
		buffer(data->err_ptr, data, (data->fmt - data->err_ptr));
}
