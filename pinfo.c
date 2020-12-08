typedef long long LL;
#include "global_vars.h"
#include "master_header.h"

void exec_pinfo(struct cmd_var *ptr)
{
    //https://unix.stackexchange.com/questions/16883/what-is-the-maximum-value-of-the-process-id

    /*A function returns the address of a stack variable, which will cause 
   unintended program behavior, typically in the form of a crash. Because local variables are allocated on the stack, 
    when a program returns a pointer to a local variable, it is returning a stack address.
    HENCE, BAD PRACTICE TO USE pid_path[100]*/
    char *pid_path = (char *)malloc(sizeof(char) * (100));
    pid_path[0] = '\0';
    strcat(pid_path, "/proc/\0");
    int query_pid;
    if (ptr->arg_num == 1)
    {
        //by default supply shell pid
        query_pid = script_pid;
        strcat(pid_path, int_to_string(query_pid));
    }
    else
    {
        query_pid = atoi(ptr->cmd_args[1]);
        strcat(pid_path, ptr->cmd_args[1]);
    }

    //     pid -- 231
    // Process Status -- {R/S/S+/Z}
    // memory -- 67854 {​Virtual Memory​}
    // Executable Path -- ~/a.out
    strcat(pid_path, "/\0");
    //printf("Path yet is %s\n", pid_path);
    ///////////////////////////////////
    //First check if pidis valid, ie whether the directory proc/pid exists
    DIR *dir = opendir(pid_path);
    if (dir != NULL)
    {
        //printf("Process id is valid as directory present in proc\n");
        closedir(dir);
    }
    else if (ENOENT == errno)
    {
        /* Directory does not exist. */
        is_legendary = 0;
        perror("opendir for proc/pid failed ");
        fprintf(stderr,"Directory corresponding to given pid does not exist\n");
        return;
    }
    else
    {
        /* opendir() failed for some other reason. */
        is_legendary = 0;
        perror("opendir for proc/pid failed ");
        fprintf(stderr,"Directory corresponding to given pid failed to open for some reason\n");
        return;
    }

    ////////////////////////////////////
    LL len_now = strlen(pid_path);
    short stat1, stat2;
    int stat3;

    char *status_str = (char *)malloc(sizeof(char) * (5));
    char *mem_str = (char *)malloc(sizeof(char) * (15));
    char *tpgid_str = (char *)malloc(sizeof(char) * (15));

    int init_buff_sz = 10000;
    char *buff = (char *)malloc(sizeof(char) * (init_buff_sz));

    //status
    stat1 = get_status(pid_path, status_str, buff);
    pid_path[len_now] = '\0';

    //memory info
    stat2 = get_memory(pid_path, mem_str, buff);
    pid_path[len_now] = '\0';

    //////////////////////
    printf("pid -- %d\n", query_pid);

    //ps -eo user,pid,pgid,tpgid,comm,stat
    //check if part of foreground terminal process grp
    int p_grp_id = getpgid(query_pid);
    int stat4 = extract_tpgid(pid_path, tpgid_str, buff);
    pid_path[len_now] = '\0';
    int t_pgid = -1;
    if (stat4 == 0)
    {
        t_pgid = (int)strtol(tpgid_str, NULL, 10);
    }

    stat3 = get_executable_path(pid_path, buff, init_buff_sz);
    pid_path[len_now] = '\0';

    if (stat1 != -1)
    {
        printf("Process Status -- %s", status_str);
        if (stat4 == 0)
        {
            if (t_pgid == p_grp_id)
            {
                printf("+");
            }
        }
        printf("\n");
    }
    else
    {
        is_legendary = 0;

        fprintf(stderr,"Error occurred while getting process status\n");
    }

    if (stat2 != -1)
    {
        //https://serverfault.com/a/908467

        //use ps aux to verify
        int ans_here = strtol(mem_str, NULL, 10);
        //1 page=4KB
        ans_here *= 4;
        printf("memory -- %d\n", ans_here);
    }
    else
    {
        is_legendary = 0;

        fprintf(stderr,"Error occurred while getting memory used\n");
    }

    if (stat3 != -1)
    {
        printf("Executable Path --  %s\n", buff);
    }
    else
    {
        is_legendary=0;
        fprintf(stderr,"Error getting executable path\n");
    }

    if (buff != NULL)
    {
        free(buff);
    }
    free(tpgid_str);
    free(status_str);
    free(mem_str);
    free(pid_path);
}
short extract_tpgid(char *pid_path, char *tpgid_str, char *buff)
{
    strcat(pid_path, "stat\0");
    int fd = open(pid_path, O_RDONLY);
    // printf("retriving tpgid\n");

    if (fd < 0)
    {
        is_legendary = 0;

        perror("Error during opening proc/pid/stat");
        return -1;
    }

    int check_status = read(fd, buff, 600);
    if (check_status < 0)
    {
        is_legendary = 0;

        perror("Some error while reading file stat:");
        return -1;
    }
    else
    {
        char delims[] = " \t\r\n";
        char *token_beg;
        int done = 0;
        token_beg = strtok(buff, delims);
        while (token_beg != NULL)
        {
            done++;
            if (done == 8)
            {
                break;
            }
            //ptr->cmd_args[ptr->arg_num++] = token_beg;
            token_beg = strtok(NULL, delims);
        }

        strcpy(tpgid_str, token_beg);

        close(fd);
        return 0;
    }
}

short get_status(char *pid_path, char *status_str, char *buff)
{
    strcat(pid_path, "stat");
    int fd = open(pid_path, O_RDONLY);
    // printf("fd is %d\n", fd);

    if (fd < 0)
    {
        is_legendary = 0;

        perror("Error during opening proc/pid/stat");
        return -1;
    }
    //printf("retriving status2\n");

    int check_status = read(fd, buff, 600);
    // printf("check status is %d\n", check_status);

    if (check_status < 0)
    {
        is_legendary = 0;

        perror("Some error while reading file stat:");
        return -1;
    }
    else
    {
        char delims[] = " \t\r\n";
        char *token_beg;
        int done = 0;
        token_beg = strtok(buff, delims);
        while (token_beg != NULL)
        {
            done++;
            if (done == 3)
            {
                break;
            }
            //ptr->cmd_args[ptr->arg_num++] = token_beg;
            token_beg = strtok(NULL, delims);
        }

        strcpy(status_str, token_beg);

        close(fd);
        // printf("outside\n");
        return 0;
    }
}

short get_memory(char *pid_path, char *mem_str, char *buff)
{
    strcat(pid_path, "statm\0");
    int fd = open(pid_path, O_RDONLY);
    // printf("retriving mem\n");

    if (fd < 0)
    {
        is_legendary = 0;

        perror("Error during opening proc/pid/stat");
        return -1;
    }

    int check_status = read(fd, buff, 600);
    if (check_status < 0)
    {
        is_legendary = 0;

        perror("Some error while reading file statm:");
        return -1;
    }
    else
    {
        char delims[] = " \t\r\n";
        char *token_beg;
        int done = 0;
        token_beg = strtok(buff, delims);
        while (token_beg != NULL)
        {
            done++;
            if (done == 1)
            {
                break;
            }
            //ptr->cmd_args[ptr->arg_num++] = token_beg;
            token_beg = strtok(NULL, delims);
        }

        strcpy(mem_str, token_beg);

        close(fd);
        return 0;
    }
}

LL get_executable_path(char *pid_path, char *buff, int now_sz)
{
    //you can read about symlinks here
    //https://stackoverflow.com/a/1531795/6427607

    //https://stackoverflow.com/a/9385927/6427607
    /*note that procs does weired stuff with st_size and hnece best bet is to keep
    expanding buff_sz till it is sufficient*/

    //Comparing ssize_t with int conundrum
    //https://stackoverflow.com/a/3642067/6427607
    strcat(pid_path, "exe\0");

    const int add_sz = 100;
    const int max_sz = 1000000;
    LL i;
    //LL len_path_executable;
    // printf("retriving executable\n");
    for (i = now_sz; i <= max_sz; i += add_sz)
    {
        int bytes_read = readlink(pid_path, buff, i);
        if (bytes_read < 0)
        {
            is_legendary = 0;

            perror("readlink error :");
            return -1;
        }
        if (bytes_read < i)
        {
            //readlink() does not append a null  byte to buf.
            buff[bytes_read] = '\0';
            return bytes_read;
        }
        else
        {
            free(buff);
            buff = (char *)malloc(sizeof(char) * (i + add_sz));
            if (buff == NULL)
            {
                is_legendary = 0;

                fprintf(stderr,"Failed to malloc sufficient len to store path of executable\n");
                return -1;
            }
        }
    }

    return -1;
}

char *int_to_string(int query_pid)
{
    /*A function returns the address of a stack variable, which will cause 
   unintended program behavior, typically in the form of a crash. Because local variables are allocated on the stack, 
    when a program returns a pointer to a local variable, it is returning a stack address.
    HENCE, BAD PRACTICE TO USE ans[15]*/
    char *ans = (char *)malloc(sizeof(char) * (15));
    int dig_num = 0;
    int tmp = query_pid;
    while (tmp)
    {
        dig_num++;
        tmp /= 10;
    }
    ans[dig_num] = '\0';
    for (int i = dig_num - 1; i >= 0; i--)
    {
        ans[i] = '0' + query_pid % 10;
        query_pid /= 10;
    }
    return ans;
}