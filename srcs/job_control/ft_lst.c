#include "struct.h"
#include "exec.h"
#include "libft.h"

int32_t			job_has_finish(void *job, void *status)
{
	t_job		*j;
	uint8_t	ending_status;

	j = job;
	ending_status = *((uint8_t *)(status));
	if (j->status & ending_status)
		return (1);
	return (0);
}
