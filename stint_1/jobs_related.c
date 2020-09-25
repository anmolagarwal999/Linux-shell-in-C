#include "global_vars.h"
#include "master_header.h"

/*add_job(job_list_t *job_list, int jid, pid_t pid, process_state_t
state, char *command)​: adds a job to the job list. Each job in the job list has a job id,
process id, state, and command (which is the command used to start that job (e.g.
/bin/sleep​)).*/

void add_job(int child_pid, struct cmd_var *ptr)
{
    //is bg cmd

    //add new job -> background to the list
    jobs_ptr[num_jobs_cmd]->pid = child_pid;
    jobs_ptr[num_jobs_cmd]->cmd_name[0] = '\0';
    jobs_ptr[num_jobs_cmd]->cmd_stat = 1; //process is running
    printf("curr job id is %d\n",curr_job_id);
    jobs_ptr[num_jobs_cmd]->jid = curr_job_id++;
    for (int i = 0; i < ptr->arg_num; i++)
    {
        strcat(jobs_ptr[num_jobs_cmd]->cmd_name, ptr->cmd_args[i]);
        strcat(jobs_ptr[num_jobs_cmd]->cmd_name, " \0");
    }

    printf("Newly created background process with pid:%d and jid: %d\n", child_pid, jobs_ptr[num_jobs_cmd]->jid);
    
    num_jobs_cmd++;
}

void exec_jobs_cmd()
{
    int i = 0;
    int n = curr_job_id;

    for (i = 0; i < n; i++)
    {
        if (jobs_ptr[i]->is_relevant == 1)
        {
            //process has not yet terminated -> either it has stopped or it is running
            int curr_stat = jobs_ptr[i]->cmd_stat;

            // [1] Running emacs assign1.txt [221]

            printf("[job_id : %d] ",jobs_ptr[i]->jid);

            if (curr_stat == 1)
            {
                printf("RUNNING ");
            }
            else
            {
                printf("STOPPED ");
            }

            printf(" %s [ pid: %d]",jobs_ptr[i]->cmd_name, jobs_ptr[i]->pid);
        }
    }
}