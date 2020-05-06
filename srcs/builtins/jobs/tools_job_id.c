#include "libft.h"
#include "exec.h"
#include "struct.h"
#include "sh.h"
#include "var.h"
#include "ft_printf.h"
#include "job_control.h"

int8_t		get_curr_id(void)
{
	int8_t	s;
	t_list	*j;

	j = cfg_shell()->job;
	s = find_stopped_job(j);
	while (j)
	{
		if ((!s && !j->next) ||
		(s == 1 && (((t_job *)j->data)->status == STOPPED)) ||
		(s > 1 && find_stopped_job(j) == 1 &&
		(((t_job *)j->data)->status == STOPPED)))
			return (((t_job *)j->data)->id);
		j = j->next;
	}
	return (0);
}

int8_t		get_prev_id(void)
{
	int8_t	s;
	t_list	*j;

	j = cfg_shell()->job;
	s = find_stopped_job(j);
	while (j)
	{
		if (!j->next || (!s && !(j->next)->next) ||
		(s == 1 && ((!j->next && (((t_job *)j->data)->status != STOPPED))
		|| (!(j->next)->next && (((t_job *)(j->next)->data)->status == STOPPED)))) ||
		(s > 1 && ((t_job *)j->data)->status == STOPPED && 
		find_stopped_job(j) == 2))
			return (((t_job *)j->data)->id);
		j = j->next;
	}
	return (0);
}

int8_t		get_sstr_id(char *ope)
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
				break;
		}
		job = job->next;
	}
	if (mm == 1)
		return (id);
	else
		return (0); // gerer les msg erreurs differents!
	return 0;
}

char		*ft_strdelc(char *str, char c)
{
	int	i;
	int	n;
	char	*rts;

	i = 0;
	n = 0;
	rts = ft_strnew(ft_strlen(str)); // allou n caractere de trop!
	while (str[i])
	{
		if (str[i] != c)
			rts[i - n] = str[i];
		else
			n++;
		i++;
	}
	return (rts);
}

int8_t		get_str_id(char *ope)
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
		if (ft_strchr(ope, '"') && !(ope = ft_strdelc(ope, '"')))
			return (0); // a prendre en compte? ya alloc!
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
		return (0); // gerer les msg erreurs differents!
}

uint8_t		get_job_id(char *ope)
{
	int8_t	ret;
	
	ret = 0;
	if (!ft_strcmp(ope, "%"))
		ret = get_curr_id();
	else if (*ope == '%')
		ope++;
	if (str_is_digit(ope))
		ret = ft_atoi(ope);
	else if (*ope == '+' || *ope == '%')
		ret = get_curr_id();
	else if (*ope == '-')
		ret = get_prev_id();
	else if (*ope == '?')
		ret = get_sstr_id(++ope);
	else
		ret = get_str_id(ope);
	return (ret);
}
