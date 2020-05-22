/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ambelghi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/23 19:29:30 by ambelghi          #+#    #+#             */
/*   Updated: 2020/03/09 19:52:38 by ambelghi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <unistd.h>
#include "struct.h"

static int	echo_options(char **av, int *i, int *j)
{
	char	*fl;
	int		ret;
	int		flags;

	flags = 0;
	if (av)
	{
		fl = "ne";
		while ((ret = ft_getopt(i, j, av, fl)) != -1)
		{
			if (ret == (int)'?')
				return ((flags |= 1));
			else if (ret == (int)'n')
				flags |= 2;
			else if (ret == (int)'e')
				flags |= 4;
		}
	}
	return (flags);
}

static char	*convert_operand(char *s)
{
	int		i;
	char	*tmp;

	if (s && (i = 0) == 0)
	{
		while (s[i])
		{
			if (s[i] == '\\' && s[i + 1] && ft_strchr("abcfnrtv", s[i + 1]))
			{
				if (s[i + 1] == 'a')
					s[i] = '\a';
				else if (s[i + 1] == 'b')
					s[i] = '\b';
				else if (s[i + 1] == 'c')
					s[i] = '\0';
				else if (s[i + 1] == 'f')
                    s[i] = '\f';
				else if (s[i + 1] == 'n')
                    s[i] = '\n';
				else if (s[i + 1] == 'r')
                    s[i] = '\r';
				else if (s[i + 1] == 't')
                    s[i] = '\t';
				else if (s[i + 1] == 'v')
                    s[i] = '\v';
				tmp = s;
				s[i + 1] = '\0';
				s = ft_strjoin(s, &s[i + 2]);
				ft_strdel(&tmp);
			}
			i++;
		}
	}
	return (s);
}

uint8_t		ft_echo(t_job *j, t_process *p) 
{
    t_point i;
    int     fl; 
    char    **params;

    if (j && p && (params = p->av))
    {
        i = (t_point){1, 0}; 
        if (((fl = echo_options(params, &i.x, &i.y)) & 1)) 
            i.x = 0;
        while (params[i.x])
        {
			if ((fl & 4)) 
                params[i.x] = convert_operand(params[i.x]);
            ft_putstr(params[i.x++]);
            ft_putstr(params[i.x] ? " " : NULL);
        }
        if (!(fl & 2)) 
            ft_putchar('\n');
        return (0);
    }   
    return (1);
}
