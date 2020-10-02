#include "master_header.h"
#include "global_vars.h"
#include <sys/select.h>

void exec_nightswatch(struct cmd_var *ptr)
{
    int till_now = 0;
    // printf("args is %d\n");
    if (ptr->arg_num != 4)
    {
        is_legendary = 0;

        fprintf(stderr, "Exactly 4 tokens must be given\n");
        return;
    }
    int choice = 1;
    //int num = -1;
    //1->interrupt
    //2->newborn
    int interval = -1;
    for (int i = 1; i < 4; i++)
    {
        if (ptr->cmd_args[i][0] == '-')
        {
            continue;
        }
        if ((strcmp(ptr->cmd_args[i], "newborn")) == 0)
        {
            choice = 2;
            continue;
        }

        char first_ch = ptr->cmd_args[i][0];
        if (first_ch >= '0' && first_ch <= '9')
        {
            interval = strtol(ptr->cmd_args[i], NULL, 10);
        }
    }

    if (interval == -1)
    {
        is_legendary = 0;

        fprintf(stderr, "Invalid args\n");
        return;
    }

    //printf("inerval is %d and ch is %d", interval, choice);
    char buff[80];
    bool goOn = true;
    while (goOn)
    {
        //https: //man7.org/linux/man-pages/man2/select.2.html
        fd_set rfds;
        struct timeval tv;
        int retval;

        /* Watch stdin (fd 0) to see when it has input. */
        /*This macro clears (removes all file descriptors from) set.  It
              should be employed as the first step in initializing a file
              descriptor set*/
        FD_ZERO(&rfds);

        /*This macro adds the file descriptor fd to set.  Adding a file
              descriptor that is already present in the set is a no-op, and
              does not produce an error.*/
        FD_SET(0, &rfds);

        tv.tv_sec = 0;
        tv.tv_usec = 0;

        retval = select(1, &rfds, NULL, NULL, &tv);

        if (retval == -1)
        {
            is_legendary = 0;

            perror("select()");
        }
        else if (retval > 0)
        {
            //printf("Data is available now %d.\n", retval);
            int n = read(0, buff, retval);
            //printf("n is %d\n", n);
            if (n > 0)
            {
                buff[n] = '\0';
                for (int j = 0; j < n; j++)
                {
                    if (buff[j] == 'q')
                    {
                        goOn = false;
                        break;
                    }
                }
            }
        }

        if (goOn == false)
        {
            break;
        }
        if (choice == 2)
        {
            printf("%lld\n", get_recent_pid());
        }
        else
        {
            display_cpu_stuff(till_now);
            till_now++;
        }

        sleep(interval);
    }
}

void display_cpu_stuff(int done_already)
{
    //strcat(pid_path, "stat\0");
    int fd = open("/proc/interrupts", O_RDONLY);
    // printf("retriving tpgid\n");

    if (fd < 0)
    {
        is_legendary = 0;

        perror("Error during opening proc/interrupts");
        return;
    }
    char *buff = (char *)malloc(500);
    int check_status = read(fd, buff, 500);
    if (check_status < 0)
    {
        is_legendary = 0;

        perror("Some error while reading file stat:");
        return;
    }
    else
    {
        //printf("check status is ");
        buff[check_status] = '\0';
        int done = 0, i = 0, j;
        for (i = 0; i < check_status; i++)
        {
            if (done == 2)
            {
                break;
            }
            if (buff[i] == '\n')
            {
                buff[i] = '\0';
                if (done_already == 0)
                {
                    printf("%s\n", buff);
                    done_already = -1;
                }
                done++;
            }
        }
        done = 0;
        for (j = i;; j++)
        {
            if (buff[j] == '\0' || buff[j] == '\n')
            {
                break;
            }
            else
            {
                if (buff[j] == '1' && buff[j + 1] == ':')
                {
                    buff[j] = buff[j + 1] = ' ';
                }
                if (buff[j] >= 'a' && buff[j] <= 'z')
                {
                    break;
                }
                if (buff[j] >= 'A' && buff[j] <= 'Z')
                {
                    break;
                }
                printf("%c", buff[j]);
            }
        }
        printf("\n");
        free(buff);

        close(fd);
        //return ans;
    }
}

LL get_recent_pid()
{

    //strcat(pid_path, "stat\0");
    int fd = open("/proc/loadavg", O_RDONLY);
    // printf("retriving tpgid\n");

    if (fd < 0)
    {
        is_legendary = 0;

        perror("Error during opening proc/loadavg");
        return -1;
    }
    char *buff = (char *)malloc(100);
    int check_status = read(fd, buff, 100);
    if (check_status < 0)
    {
        is_legendary = 0;

        perror("Some error while reading file loadavg:");
        return -1;
    }
    else
    {

        buff[check_status] = '\0';
        int done = 0, i = 0;
        for (i = 0; i < check_status; i++)
        {
            if (done == 4)
            {
                break;
            }
            if (buff[i] == ' ')
            {
                done++;
            }
        }
        LL ans = strtol(&buff[i], NULL, 10);
        free(buff);

        close(fd);
        return ans;
    }
}