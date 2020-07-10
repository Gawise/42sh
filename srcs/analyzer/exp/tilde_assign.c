/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tilde_assign.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/10 14:45:39 by user42            #+#    #+#             */
/*   Updated: 2020/07/10 14:45:50 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "analyzer.h"
#include "libft.h"

void	tilde_assign_dispatch(char **str, t_exp *exp, int *state)
{
	if (*state == 0 && **str == '~')
	{
		exp_flush_buf(exp, &exp->res);
		parse_tilde_exp(str, exp, 1);
		ft_bzero(exp->buf, EXP_BSIZE);
		exp->i = 0;
	}
	else if (ft_strchr("\'\"\\", **str))
	{
		if (**str == '\'' && exp->quote < 2)
			exp->quote = exp->quote == 1 ? 0 : 1;
		else if (**str == '\"' && exp->quote != 1)
			exp->quote = exp->quote == 2 ? 0 : 2;
		else if (**str == '\\' && exp->quote != 1)
			exp->bs = 2;
		*state = 1;
	}
	else if (**str == ':')
		*state = -1;
}

void	find_tilde_exp_assign(char **word, t_exp exp)
{
	char	*str;
	int		state;

	if (!word || !*word)
		return ;
	str = *word;
	state = 0;
	while (*str)
	{
		if (!exp.bs && ft_strchr("~\'\"\\:", *str))
			tilde_assign_dispatch(&str, &exp, &state);
		if (exp.i >= EXP_BSIZE - 1)
			exp_flush_buf(&exp, &exp.res);
		exp.buf[exp.i] = *str;
		if (exp.bs)
			exp.bs--;
		if (state < 1)
			state++;
		str++;
		exp.i++;
	}
	exp_flush_buf(&exp, &exp.res);
	free(*word);
	*word = exp.res;
}
