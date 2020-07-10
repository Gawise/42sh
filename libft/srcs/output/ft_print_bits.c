/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_bits.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/10 15:09:56 by user42            #+#    #+#             */
/*   Updated: 2020/07/10 15:09:56 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

void	ft_print_bits(unsigned char nb)
{
	int	i;
	int	c;

	i = 128;
	c = 0;
	while (nb > 0)
	{
		if (nb / i > 0)
		{
			ft_putchar('1');
			nb = nb - i;
			c++;
		}
		else
		{
			ft_putchar('0');
			c++;
		}
		i = i / 2;
	}
	while (c < 8)
	{
		ft_putchar('0');
		c++;
	}
}
