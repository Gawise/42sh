#include "libft.h"
#include "exec.h"
#include "struct.h"

int8_t		find_stopped_job(t_list *job)
{
	int	i;

	i = 0;
	while (job)
	{
		if (((t_job *)job->data)->status == STOPPED)
			i++;
		job = job->next;
	}
	return (i);
}

char		get_curr_multi(t_list *job, uint8_t nb)
{
	if (((t_job *)job->data)->status != STOPPED)
		return (' ');
	else
	{
		if (nb == 1)
			return ('+');
		else if (nb == 2)
			return ('-');
		else
			return (' ');
	}
}

char		get_curr(t_list *job, uint8_t stop)
{
	uint8_t	nb;

	nb = find_stopped_job(job);
	if (stop == 0)
	{
		if (!job->next)
			return ('+');
		else if (!(job->next)->next)
			return ('-');
		else
			return (' ');
	}
	else if (stop == 1)
	{
		if (((t_job *)job->data)->status == STOPPED)
			return ('+');
		else if ((!job->next) || (!(job->next)->next &&
		(((t_job *)(job->next)->data)->status == STOPPED)))
			return ('-');
		else
			return (' ');
	}
	else
		return (get_curr_multi(job, nb));
}

int			str_is_digit(char *ope)
{
	while (*ope)
		if (!ft_isdigit(*ope++))
			return (0);
	return (1);
}
