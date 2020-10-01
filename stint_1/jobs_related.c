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
    *idx_in_table = -1;
    for (int i = 0; i < num_jobs_cmd; i++)
    {
        if (jobs_ptr[i]->is_relevant == 1)
        {
            if (jobs_ptr[i]->jid == search_jid)
            {
                *pid_fetched = jobs_ptr[i]->pid;
                *idx_in_table = i;
            }
        }
    }
}

void add_job(int child_pid, struct cmd_var *ptr, int curr_cmd_stat)
{
    //is bg cmd

    //add new job -> background to the list
    jobs_ptr[num_jobs_cmd]->pid = child_pid;
    jobs_ptr[num_jobs_cmd]->cmd_name[0] = '\0';
    jobs_ptr[num_jobs_cmd]->cmd_stat = curr_cmd_stat; //process is running if 1, stopped if 0
    jobs_ptr[num_jobs_cmd]->is_relevant = 1;          //process not yet terminated

    printf("curr job id is %d\n", curr_job_id);
    jobs_ptr[num_jobs_cmd]->jid = curr_job_id++;
    for (int i = 0; i < ptr->arg_num; i++)
    {
        strcat(jobs_ptr[num_jobs_cmd]->cmd_name, ptr->cmd_args[i]);
        strcat(jobs_ptr[num_jobs_cmd]->cmd_name, " \0");
    }

    //made change that if curr_stat is 1-> running, print nothing as in shell, if curr_stat is 0, print STOPPED
    printf("Process with pid:[%d] and jid: [%d]", child_pid, jobs_ptr[num_jobs_cmd]->jid);
    if (curr_cmd_stat == 0)
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

void exec_fg(struct cmd_var *ptr)
{
    // struct cmd_var
    // {
    //     int arg_num;
    //     char *cmd_args[max_poss_args];
    //     int is_bg;
    // };

    printf("number of args are %d\n", ptr->arg_num);

    if (ptr->arg_num != 2)
    {
        fprintf(stderr, "Invalid number of args for fg\n");
        return;
    }

    int job_id = atoi(ptr->cmd_args[1]);

    if (job_id - 1 < 0 || job_id - 1 > curr_job_id - 1)
    {
        fprintf(stderr, "Invalid job id\n");
        return;
    }

    int job_idx = job_id - 1;
    if (jobs_ptr[job_idx]->is_relevant == 0)
    {
        fprintf(stderr, "Job with this job id is neither running nor stopped\n");
        return;
    }

    ///////////////////////////////////////////////////////////////////////////////////////

    //It's Child is FOREGROUND PROCESS
    //fprintf(stderr,"in parent\n");
    int child_pid = jobs_ptr[job_idx]->pid;
    //fflush(stdout);
    curr_fg_pid = child_pid;

    //https://www.gnu.org/software/libc/manual/html_node/Foreground-and-Background.html#Foreground-and-Background

    /* When child is foreground job , parent must first give 
           child access to the controlling terminal.*/

    //0 is fd for STDIN
    setpgid(child_pid, 0);
    tcsetpgrp(0, child_pid);

    /*SInce, child is fg and wants control of terminal,
            parent needs to ignore the fact that it itself can't read input or give output*/
    //Otherwise, parent would stop.
    signal(SIGTTOU, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);

    // If pid is greater than 0, waitpid() waits for termination of the specific child whose process ID is equal to pid.
    // fprintf(stderr,"going to wait\n");

    /*When sent to a stopped process, this signal causes the process to resume 
(i.e., to be rescheduled to run at some later time). When received by a process that is not currently stopped, 
this signal is ignored by default.
 A process may catch this signal, so that it carries out some action when it resumes.*/

    kill(child_pid, SIGCONT);
    fflush(stdout);
    int fg_stat;
    short stat_temp = waitpid(child_pid, &fg_stat, WUNTRACED);

    printf("fg_stat after waitpid is is %d\n", fg_stat);

    if (stat_temp == -1)
    {
        perror("Error while wait():");
    }

    //  fprintf(stderr,"wait over\n");
    fflush(stdout);

    //Restore control of terminal to self
    tcsetpgrp(0, getpid());

    //Since terminal restores to self,no longer ignore signals
    signal(SIGTTOU, SIG_DFL);
    signal(SIGTTIN, SIG_DFL);
    //exit(0);

    printf("Wait over\n");
    curr_fg_pid = -1;

    /* WIFSTOPPED(wstatus)
              returns true if the child process was stopped by delivery of a
              signal; this is possible only if the call was done using WUN‐
              TRACED or when the child is being traced (see ptrace(2)).*/

    if (WIFSTOPPED(fg_stat))
    {
        //bg job with status=stopped
        jobs_ptr[job_idx]->is_relevant = 1;
        int stopping_signal_number = WSTOPSIG(fg_stat);
        int i=job_idx;
        jobs_ptr[i]->cmd_stat = 2;
        fprintf(stderr, "%s with pid [%d] and jid [%d] -> STOPPED with signal number %d\n", jobs_ptr[i]->cmd_name, jobs_ptr[i]->pid, jobs_ptr[i]->jid, stopping_signal_number);
        jobs_ptr[job_idx]->cmd_stat = 2;
    }
    else
    {
        //bg job which was brought to fg has exited
        jobs_ptr[job_idx]->is_relevant = 0;
    }

    /////////////////////////////////////////////////////////////////////////////////////
}