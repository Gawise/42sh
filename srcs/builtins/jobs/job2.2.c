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

uint8_t		get_curr_id(t_list ljob, uint8_t id)
{
	t_job		*j;
	
	while (ljob)
	{
		j = jlob->data;
		if (id == j->id)
			return (
	}
	

	return (0);
}

int			str_is_digit(char *ope)
{
	while (*ope)
		if (!ft_isdigit(*ope++))
			return (0);
	return (1);
}

int16_t		get_job_id(char *ope)
{
	int8_t	ret;

	ft_isnumeric();
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

uint8_t		print_job(t_cfg *shell, t_process *p, char opti, int32_t ac)
{
	uint8_t		i;

	i = 0;
	while (p->av[ac])
	{
		ope = get_job_id(p->av[ac]);
		if (!print_job_ope(opt, j, ope))
		{
			if (ope == -1)
			{
				ft_dprintf(STDERR_FILENO, "21sh: jobs: %s", p->av[ac]);
				ft_dprintf(STDERR_FILENO, ": ambiguous job spec\n");
			}
			ft_dprintf(STDERR_FILENO, "21sh: jobs: %s", p->av[ac]);
			ft_dprintf(STDERR_FILENO, ": no such job\n");
			ret++;
		}
		ac++;
	}
	return (ret ? 1 : 0);


}
}
