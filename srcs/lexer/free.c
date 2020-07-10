/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/10 14:46:13 by user42            #+#    #+#             */
/*   Updated: 2020/07/10 14:46:13 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_printf.h"
#include "lexer.h"
#include "sh.h"

void	del_flag_queue(void *data, size_t size)
{
	(void)size;
	ft_memdel(&data);
}

void	del_token(void *data, size_t size)
{
	t_token	*token;

	(void)size;
	token = (t_token *)data;
	ft_strdel(&token->str);
	free(data);
}

void	tabfree(char **tab)
{
	int	i;

	i = 0;
	if (!tab || !*tab)
		return ;
	while (tab[i])
	{
		ft_strdel(&tab[i]);
		i++;
	}
	free(tab);
}

void	del_str(void *data, size_t size)
{
	(void)size;
	free(data);
}
