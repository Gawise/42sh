/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ambelghi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/26 18:14:13 by ambelghi          #+#    #+#             */
/*   Updated: 2020/06/27 18:22:20 by ambelghi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "line_edition.h"
#include "struct.h"
#include "ft_printf.h"
#include "sh.h"

char	*set_prompt(char *s)
{
	static char	*prompt = NULL;

	if (s)
		prompt = s;
	return (prompt);
}

int	rev_i_search(t_cs_line *cs)
{
	if (cs && cs->ctrl_r == 0)
	{
		set_prompt(cs->prompt);
		cs->prompt = ft_strdup("(reverse-i-search)> ");
		cs->ctrl_r = 1;
		ft_clear(1);
		print_prompt(cs);
	}
	return (0);
}

int	ctrl_r_off(t_cs_line *cs, char *caps)
{
	(void)caps;
	if (cs && cs->ctrl_r == 1)
	{
		ft_strdel(&cs->prompt);
		cs->prompt = set_prompt(NULL);
		cs->ctrl_r = 0;
	}
	return (0);
}

int	del_char(char **del, t_dlist *hist, t_cs_line *cs)
{
	int		i;
	char	*tmp;

	if (del && *del && (i = ft_strlen(*del)))
	{
		ft_putnbr(i);
		tmp = *del;
		tmp[i - 1] = '\0';
		*del = ft_strdup(tmp);
		ft_strdel(&tmp);
		ft_clear(1);
		print_prompt(cs);
        ft_dprintf(cs->tty, "\"%s\" : %s", *del, (hist ? (char *)hist->data : NULL));
		return (1);
	}
	return (0);
}

int	history_search(t_cs_line *cs, char *caps)
{
	static char		*input = NULL;
	static t_dlist	*hist = NULL;
	char			*tmp;

	if (cs && caps && ft_strcmp(caps, "\n") != 0 && !cs->sig_int)
	{
		if (caps[0] == (char)127)
			return (del_char(&input, hist, cs));
		if (input)
		{
			tmp = input;
			ft_asprintf(&input, "%s%s", tmp, caps);
			ft_strdel(&tmp);
		}
		else
			input = ft_strdup(caps);
		if (!hist || !hist->prev || !hist->data)
			hist = cfg_shell()->history;
		while (hist->next)
			hist = hist->next;
		while (hist->prev && !ft_strstr((char *)hist->data, input))
			hist = hist->prev;
		ft_clear(1);
		print_prompt(cs);
		ft_dprintf(cs->tty, "\"%s\" : %s", input, (char *)hist->data);
	}
	else if (cs)
	{
		ctrl_r_off(cs, caps);
		ft_strdel(&input);
		if (hist && hist->data)
		{
		//	ft_strdel(&cs->input);
			cs->input = ft_strdup((char *)hist->data);
		}
		hist = NULL;
	}
	return (0);
}
