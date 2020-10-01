#include "master_header.h"
#include "global_vars.h"

void reapChild(int signum)
{
    int i;
    int *stat_loc = (int *)malloc(sizeof(int));
    printf("Entered reap Child\n");
    if (getpid() != script_pid)
    {
        return;
    }
    // looping through this as multiple childExits will still cause single invocation of reapChild
    while (true)
    {
        //printf("IN LOOP\n");
        pid_t suspected_pid = waitpid(-1, stat_loc, WNOHANG);
        printf("Suspected pid is %d\n",suspected_pid);
        if (suspected_pid <= 0)
        {
            /*if WNOHANG was specified and one or more
            child(ren) specified by pid exist, but have not yet changed state,
            then 0 is returned.*/
            break;
        }
        else
        {
            for (i = 0; i < num_jobs_cmd; i++)
            {
                //printf("i is %d\n", i);
                if (jobs_ptr[i]->pid == suspected_pid)
                {
                    //genuine process
                    //  printf("genuine process with pid %d\n", jobs_ptr[i]->pid);
                    if (jobs_ptr[i]->cmd_stat == 1 || jobs_ptr[i]->cmd_stat == 2)
                    {
                        //possible change in behaviour or has exited




                        //iF process terminated, 
                        //print apt message and make sure that is not considered again

                        //else, proces has undergone a change in status, update the change in status accrdingly, 
                        //also print about the change in status

                        //////////////////////////////////////////////////////////
                        // [<jid>] (<pid>) terminated with exit status <status>
                        //////////////////////////////////////////////////////////////
                        if (WIFEXITED(*stat_loc) && WEXITSTATUS(*stat_loc) == EXIT_SUCCESS)
                        {
                            //  https://stackoverflow.com/questions/39002052/how-i-can-print-to-stderr-in-c
                            jobs_ptr[i]->is_relevant=0;
                            fprintf(stderr, "%s with pid [%d] and jid [%d] exited normally\n", jobs_ptr[i]->cmd_name, jobs_ptr[i]->pid, jobs_ptr[i]->jid);
                        }
                        else
                        {
                            fprintf(stderr, "%s with pid [%d] and jid [%d] exited ABNORMALLY with status as %d\n", jobs_ptr[i]->cmd_name, jobs_ptr[i]->pid, jobs_ptr[i]->pid, WEXITSTATUS(*stat_loc));
                        }

                        break;
                    }
                }
            }
        }

        signal(SIGCHLD, reapChild);
    }
}