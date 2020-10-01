#ifndef JOBS_RELATED_HEADER
#define JOBS_RELATED_HEADER

void add_job(int child_pid, struct cmd_var *ptr,int curr_cmd_stat);
void exec_jobs_cmd();

#endif