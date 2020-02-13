/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ambelghi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/11 20:32:03 by ambelghi          #+#    #+#             */
/*   Updated: 2020/02/11 22:59:05 by ambelghi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "line_edition.h"

t_dlist	*init_history(void)
{
	t_dlist	*hist;

	hist = ft_dlstnew(NULL, 0);
	hist->data = NULL;
	return (hist);
}

void	history_up(t_cs_line *cs)
{
	if (cs)
	{
		if (cs->history->prev && cs->history->prev->data)
		{
			cs->history = cs->history->prev;
			cs->input = ft_strnew(0);
			cs->line_col = 0;
			line_master(cs, (char *)cs->history->data);
			cs->line_col = ft_strlen(cs->input);
			print_cmdline(cs);
		}
	}
}

void    history_down(t_cs_line *cs)
{
    if (cs)
    {
        if (cs->history->next && cs->history->next->data)
        {
            cs->history = cs->history->next;
            cs->input = (char *)cs->history->data;
            cs->line_col = ft_strlen(cs->input);
            print_cmdline(cs);
        }
    }
}
