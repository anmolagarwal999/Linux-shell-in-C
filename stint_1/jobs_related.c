#include "global_vars.h"
#include "master_header.h"

/*add_job(job_list_t *job_list, int jid, pid_t pid, process_state_t
state, char *command)​: adds a job to the job list. Each job in the job list has a job id,
process id, state, and command (which is the command used to start that job (e.g.
/bin/sleep​)).*/

// struct jobs_cmd
// {
//     int pid,jid;
//     char cmd_name[100];
//     int cmd_stat;  //running or stopped 1->running, 2 -> stopped
//     int is_relevant; // is 1 only if it is not terminated yet, becomes 0 if it has been terminated
// };

//: gets the process id of a job indicated by a job id.

void get_job_pid(int search_jid, int *pid_fetched, int *idx_in_table)
{
    *idx_in_table=-1;
    for (int i = 0; i < num_jobs_cmd; i++)
    {
        if (jobs_ptr[i]->is_relevant == 1)
        {
            if (jobs_ptr[i]->jid == search_jid)
            {
                *pid_fetched = jobs_ptr[i]->pid;
                *idx_in_table=i;
            }
        }
    }
}

void add_job(int child_pid, struct cmd_var *ptr,int curr_cmd_stat)
{
    //is bg cmd

    //add new job -> background to the list
    jobs_ptr[num_jobs_cmd]->pid = child_pid;
    jobs_ptr[num_jobs_cmd]->cmd_name[0] = '\0';
    jobs_ptr[num_jobs_cmd]->cmd_stat = curr_cmd_stat;    //process is running if 1, stopped if 0
    jobs_ptr[num_jobs_cmd]->is_relevant = 1; //process not yet terminated

    printf("curr job id is %d\n", curr_job_id);
    jobs_ptr[num_jobs_cmd]->jid = curr_job_id++;
    for (int i = 0; i < ptr->arg_num; i++)
    {
        strcat(jobs_ptr[num_jobs_cmd]->cmd_name, ptr->cmd_args[i]);
        strcat(jobs_ptr[num_jobs_cmd]->cmd_name, " \0");
    }

    //made change that if curr_stat is 1-> running, print nothing as in shell, if curr_stat is 0, print STOPPED
    printf("Process with pid:[%d] and jid: [%d]", child_pid, jobs_ptr[num_jobs_cmd]->jid);
    if(curr_cmd_stat==0)
    {
        //stopped
        printf("\t STOPPED");
    }
    printf("\n");

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

            printf("[job_id : %d] ", jobs_ptr[i]->jid);

            if (curr_stat == 1)
            {
                printf("RUNNING ");
            }
            else
            {
                printf("STOPPED ");
            }

            printf(" %s [ pid: %d]", jobs_ptr[i]->cmd_name, jobs_ptr[i]->pid);
        }
    }
}