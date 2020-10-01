#ifndef JOBS_RELATED_HEADER
#define JOBS_RELATED_HEADER

void add_job(int child_pid, struct cmd_var *ptr,int curr_cmd_stat);
void exec_jobs_cmd();
void exec_fg(struct cmd_var *ptr);


#endif