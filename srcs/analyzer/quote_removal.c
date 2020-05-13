#include "libft.h"
#include "ft_printf.h"
#include "lexer.h"
#include "parser.h"
#include "analyzer.h"
#include "var.h"
#include "line_edition.h"
#include "sh.h"

static void	a_handle_backslash(char **str, char **tmp, int *flag)
{
	if (*flag == '\\' || *flag == '\'')
	{
		**tmp = '\\';
		if (*flag == '\\')
			*flag = 0;
	}
	else if (*flag == '\"')
	{
		if (ft_strchr("\"\\", str[0][1]))
		{
			str[0]++;
			**tmp = **str;
		}
		else
			**tmp = **str;
	}
	else
	{
		*flag = '\\';
		return ;
	}
	tmp[0]++;
}

static void	a_handle_d_quote(char **tmp, int *flag)
{
	if (*flag == '\\')
		**tmp = '\"';
	else if (*flag == '\'')
		**tmp = '\"';
	else if (*flag == '\"')
	{
		*flag = 0;
		return ;
	}
	else
	{
		*flag = '\"';
		return ;
	}
	tmp[0]++;
}

static void	a_handle_s_quote(char **tmp, int *flag)
{
	if (*flag == '\\')
		**tmp = '\\';
	else if (*flag == '\'')
	{
		*flag = 0;
		return ;
	}
	else if (*flag == '\"')
		**tmp = '\'';
	else
	{
		*flag = '\'';
		return ;
	}
	tmp[0]++;
}

void	a_flag_handle(char **str, char **tmp, int *flag)
{
	if (**str == '\'')
		a_handle_s_quote(tmp, flag);
	else if (**str == '\"')
		a_handle_d_quote(tmp, flag);
	else if (**str == '\\')
		a_handle_backslash(str, tmp, flag);
	else
	{
		if (*flag == '\\')
			*flag = 0;
		**tmp = **str;
		tmp[0]++;
	}
}

char	*a_quote_removal(char **str)
{
	char	*tmp;
	char	*res;
	int		i;
	int		flag;
	char	*cpy;

	flag = 0;
	i = 0;
	cpy = *str;
	if (!(res = ft_memalloc(sizeof(char) * ft_strlen(*str))))
		ft_ex(EXMALLOC);
	tmp = res;
	while (*cpy)
	{
		if (flag > 0 || ft_strchr("\'\"\\", *cpy))
			a_flag_handle(&cpy, &tmp, &flag);
		else
		{
			*tmp = *cpy;
			tmp++;
		}
		cpy++;
	}
	ft_printf("res quote removal : %s\n", res);
	ft_strdel(str);
	return (res);
}
