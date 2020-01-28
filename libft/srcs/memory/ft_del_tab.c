/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_del_tab.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmerieux <hmerieux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/12 18:48:51 by hmerieux          #+#    #+#             */
/*   Updated: 2020/01/15 17:21:43 by hmerieux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void		ft_del_tab(void **tab)
{
	char	**target;
	int		i;

	i = 0;
	if (!tab || !*tab)
		return ;
	target = (char **)tab;
	while (target[i])
		ft_strdel(&target[i++]);
	free(target);
}
