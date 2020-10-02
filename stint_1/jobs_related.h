#ifndef JOBS_RELATED_HEADER
#define JOBS_RELATED_HEADER

void add_job(int child_pid, struct cmd_var *ptr,int curr_cmd_stat);
void exec_jobs_cmd();
void exec_fg(struct cmd_var *ptr);
void exec_bg(struct cmd_var *ptr);
void exec_kjob(struct cmd_var *ptr);
void exec_overkill(struct cmd_var *ptr);
short get_pid_status(int query_pid);



#endif