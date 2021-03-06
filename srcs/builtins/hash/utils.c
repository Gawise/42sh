/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/10 14:45:57 by user42            #+#    #+#             */
/*   Updated: 2020/07/10 14:45:57 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	hash_usage_error(void)
{
	ft_dprintf(2, "hash: usage: hash [-r] [name ...]\n");
}

int		hash_not_found(char *cmd)
{
	ft_dprintf(2, "hash: no such command: %s\n", cmd);
	return (0);
}
