/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_recursive_power.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmerieux <hmerieux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/12 18:44:05 by hmerieux          #+#    #+#             */
/*   Updated: 2020/01/12 18:44:07 by hmerieux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int		ft_recursive_power(int nb, int pw)
{
	if (pw < 0)
		return (0);
	else if (pw == 0)
		return (1);
	else
		return (ft_recursive_power(nb, pw - 1) * nb);
}
