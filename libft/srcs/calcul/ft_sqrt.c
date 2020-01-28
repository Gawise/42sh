/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sqrt.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmerieux <hmerieux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/12 18:44:33 by hmerieux          #+#    #+#             */
/*   Updated: 2020/01/12 18:44:35 by hmerieux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int		ft_sqrt(int nb)
{
	int		sqrt;

	sqrt = 1;
	if (nb < 0)
		return (0);
	while (sqrt * sqrt < nb)
		sqrt++;
	if (sqrt * sqrt == nb)
		return (sqrt);
	return (0);
}
