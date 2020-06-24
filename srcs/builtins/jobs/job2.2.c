#include "libft.h"
#include "ft_printf.h"
#include "exec.h"
#include "sh.h"

# define curr_job1 %%
# define curr_job2 %+
# define prev_job %-
# define nb_job %n
# define begin_job %string
# define contain_job %?string

int16_t		get_job_id(t_list *ljob, uint8_t nb, t_job **j, uint8_t *curr)
{
	while (ljob)
	{
		*curr += 2;
		*j = ljob->data;
		if ((*j)->id == nb)
			return (SUCCESS);
		ljob = ljob->next;
	}
	*j = NULL;
	return (FAILURE);
}

int16_t		get_curr_job(t_list *ljob, uint8_t id, t_job **j, uint8_t *curr)
{
	while (ljob && id)   //ameliorer cette boucle
	{
		*curr += 2;
		*j = ljob->data;
		ljob = ljob->next;
		id--;
	}
	if (*j)
		return (SUCCESS);
	else
		return (FAILURE);  //ternaire ici
}

int16_t		str_is_digit(char *ope)
{
	while (*ope)
		if (!ft_isdigit(*ope++))
			return (0);
	return (1);
}

int16_t		get_sstr_id(t_list *ljob, char *ope, t_job **j, uint8_t *curr)
{
	while (ljob)
	{
		*curr += 2;
		*j = ljob->data;
		if (ft_strstr((*j)->cmd, ope))
			return (SUCCESS);
		ljob = ljob->next;
	}
	return (FAILURE);
}

int16_t		get_str_id(t_list *ljob, char *ope, t_job **j, uint8_t *curr)
{
	int8_t	i;

	while (ljob)
	{
		i = 0;
		*j = ljob->data;
		while (ope[i] && (*j)->cmd[i] && ope[i] == (*j)->cmd[i])
		{
			*curr += 2;
			i++;
		}
		if (!ope[i])
			return (SUCCESS);
		ljob = ljob->next;
	}
	*j = NULL;
	return (FAILURE);
}


int16_t		get_job(t_list *ljob, char *ope, t_job **j, uint8_t *curr)

{
	int8_t	ret;

	ret = 0;

	if (!ft_strcmp(ope, "%"))
		return (get_curr_job(ljob, 1, j, curr));
	if (*ope == '%')
		ope++;
	if (str_is_digit(ope))
		return (get_job_id(ljob, ft_atoi(ope), j, curr));
	if (*ope == '+' || *ope == '%')
		return (get_curr_job(ljob, 1, j, curr));
	if (*ope == '-')
		return (get_curr_job(ljob, 2, j, curr));
	if (*ope == '?')
		return (get_sstr_id(ljob, ++ope, j, curr));
	return (get_str_id(ljob, ope, j, curr));
}

uint8_t		print_jobs(t_cfg *shell, t_process *p, char opt, int32_t ac)
{
	uint8_t		i;
	uint8_t		err;
	uint8_t		curr;
	int16_t		ret;
	t_job		*j;

	i = 0;
	err = 0;
	j = 0;
	while (p->av[ac])
	{
		curr = 41;
		if ((ret = get_job(shell->job, p->av[ac], &j, &curr)) == SUCCESS)
			print_this_job(j, curr, opt);		 //gestion des curr
		else
		{
			if (ret == -1)
			{
				ft_dprintf(STDERR_FILENO, "21sh: jobs: %s", p->av[ac]);
				ft_dprintf(STDERR_FILENO, ": ambiguous job spec\n");
			}
			ft_dprintf(STDERR_FILENO, "21sh: jobs: %s", p->av[ac]);
			ft_dprintf(STDERR_FILENO, ": no such job\n");
			err++;
		}
		ac++;
	}
	return (err ? 1 : 0);
}
