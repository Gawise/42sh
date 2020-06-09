#ifndef JOB_CONTROL_H
# define JOB_CONTROL_H

int32_t		job_has_finish(void *job, void *status);

void		add_job_cfg(t_job *job);
void		set_job_background(t_job *job);

void		ft_cpy_job(t_job *job, t_job *copy);
void		check_child(t_cfg *shell, t_list *lstjob);

void		aplylyse_wstatus(t_process *p, int wstatus);
void		update_process(t_list *lst, pid_t child, int wstatus);

t_process	*find_process_by_status(t_list *lst, uint8_t want);
t_process	*find_process_by_pid(t_list *lst, pid_t child);

void		update_listjob(t_cfg *shell);

#endif
