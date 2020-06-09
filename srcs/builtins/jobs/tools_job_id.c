#include "libft.h"
#include "exec.h"
#include "struct.h"
#include "sh.h"
#include "var.h"
#include "ft_printf.h"
#include "job_control.h"

uint8_t		get_curr_id(char c)
{
	t_list	*j;

	j = cfg_shell()->job;
	while (j)
	{
		if (((t_job *)j->data)->cur == c)
			return (((t_job *)j->data)->id);
		j = j->next;
	}
	return (0);
}

uint8_t		get_sstr_id(char *ope)
{
	t_list	*job;
	int8_t	mm;
	int8_t	id;

	mm = 0;
	id = 0;
	job = cfg_shell()->job;
	while (job)
	{
		if (ft_strstr(((t_job *)job->data)->cmd, ope))
		{
			id = (((t_job *)job->data)->id);
			if (++mm > 1)
				break ;
		}
		job = job->next;
	}
	if (mm == 1)
		return (id);
	else
		return (mm ? -1 : 0);
}

int16_t		get_str_id(char *ope)
{
	t_list	*job;
	int8_t	mm;
	int8_t	id;
	int8_t	i;

	mm = 0;
	id = 0;
	job = cfg_shell()->job;
	while (job)
	{
		i = 0;
		while (ope[i] && ope[i] == ((t_job *)job->data)->cmd[i])
			i++;
		if (ope[i] == '\0')
		{
			mm++;
			id = (((t_job *)job->data)->id);
		}
		job = job->next;
	}
	if (mm == 1)
		return (id);
	else
		return (mm ? -1 : 0);
}

int16_t		get_job_id(char *ope)
{
	int8_t	ret;

	ret = 0;
	if (!ft_strcmp(ope, "%"))
		ret = get_curr_id('+');
	else if (*ope == '%')
		ope++;
	if (str_is_digit(ope))
		ret = ft_atoi(ope);
	else if (*ope == '+' || *ope == '%')
		ret = get_curr_id('+');
	else if (*ope == '-')
		ret = get_curr_id('-');
	else if (*ope == '?')
		ret = get_sstr_id(++ope);
	else
		ret = get_str_id(ope);
	return (ret);
}
