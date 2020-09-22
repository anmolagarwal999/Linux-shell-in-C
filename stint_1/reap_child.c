#include "master_header.h"
#include "global_vars.h"

void reapChild(int signum)
{
    int i;
    int *stat_loc = (int *)malloc(sizeof(int));
    if (getpid() != script_pid)
    {
        return;
    }
    // looping through this as multiple childExits will still cause single invocation of reapChild
    while (true)
    {
        //printf("IN LOOP\n");
        pid_t suspected_pid = waitpid(-1, stat_loc, WNOHANG);
        if (suspected_pid <= 0)
        {
            /*if WNOHANG was specified and one or more
            child(ren) specified by pid exist, but have not yet changed state,
            then 0 is returned.*/
            break;
        }
        else
        {
            for (i = 0; i < num_bg_cmd; i++)
            {
                //printf("i is %d\n", i);
                if (bg_ptr[i]->pid == suspected_pid)
                {
                    //genuine process
                    //  printf("genuine process with pid %d\n", bg_ptr[i]->pid);
                    if (bg_ptr[i]->cmd_stat == 1)
                    {
                        //possible change in behaviour
                        bg_ptr[i]->cmd_stat = 0;

                        if (WIFEXITED(*stat_loc) && WEXITSTATUS(*stat_loc) == EXIT_SUCCESS)
                        {
                            //  https://stackoverflow.com/questions/39002052/how-i-can-print-to-stderr-in-c
                            fprintf(stderr, "%s with pid %d exited normally\n", bg_ptr[i]->cmd_name, bg_ptr[i]->pid);
                        }
                        else
                        {
                            fprintf(stderr, "%s with pid %d exited ABNORMALLY with status as %d\n", bg_ptr[i]->cmd_name, bg_ptr[i]->pid, WEXITSTATUS(*stat_loc));
                        }

                        break;
                    }
                }
            }
        }

        signal(SIGCHLD, reapChild);
    }
}