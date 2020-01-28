/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmerieux <hmerieux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/12 18:49:51 by hmerieux          #+#    #+#             */
/*   Updated: 2020/01/12 18:49:52 by hmerieux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

void	ft_putnbr(int n)
{
	if (n == -2147483648)
	{
		ft_putnbr(-2);
		ft_putnbr(147483648);
		return ;
	}
	if (n >= 0 && n < 10)
		ft_putchar(n + 48);
	else if (n < 0)
	{
		ft_putchar('-');
		ft_putnbr(n * (-1));
	}
	else
	{
		ft_putnbr(n / 10);
		ft_putnbr(n % 10);
	}
}
