#include "libft.h"
#include "exec.h"
#include "job_control.h"
#include "sh.h"

int32_t			focus_job(void *data1, void *data2)
{
	t_job *j;

	j = data1;
	if (j->pgid  == *(pid_t * )(data2))
		return (1);
	return (0);

}

int32_t			job_has_finish(void *job, void *status)
{
	t_job	*j;
	uint8_t	ending_status;

	j = job;
	ending_status = *((uint8_t *)(status));
	if (j->status & ending_status)
		return (1);
	return (0);
}

void		job_become_cur(t_cfg *shell, t_job *j)
{
	t_job	jc;

	ft_cpy_job(j, &jc);
	protect_malloc(&jc);
	ft_lstdelif(&shell->job, &j->pgid, focus_job, del_struct_job);
	ft_lst_push_front(&shell->job, &jc, sizeof(t_job));
}
