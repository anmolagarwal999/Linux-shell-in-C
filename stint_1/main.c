#include "master_header.h"

//////////////////////////////////////////////////////////////////////////////

char home_dir_path[1024];
char curr_dir_path[1024];
char prev_dir_path[1024];
char shell_dir_path[1024];
char history_file_path[1024];
char *hist_cmds[22];
char username_ans[200];
char hostname_ans[500];

int script_pid, num_jobs_cmd, curr_history_num, curr_job_id, curr_fg_pid, is_legendary;

//maximum number of jobs over the entire session of the shell
#define jobs_ptr_sz 100
struct jobs_cmd *jobs_ptr[jobs_ptr_sz];

void sigint_handler()
{
    // printf("inside SIGINT HANDLER\n");
    //  printf("pid of invoking process is %d\n", getpid());
    // printf("script_pid is %d\n", script_pid);
    if (getpid() == script_pid)
    {
        // part2;
        // printf("Weird stuff is happening\n");
        yellow_color();
        printf("\nNo foreground process to terminate\n");
        reset_color();
        display_prompt(username_ans, hostname_ans);
        fflush(stdout);

        // part2;
    }
    else
    {
        part2;
        red_color();
        printf("Weird stuff is happening\n");
        reset_color();
        fflush(stdout);

        part2;
    }
}

void sigtstp_handler()
{
    // printf("SIGTSTP HANDLER\n");
    // printf("pid live is %d\n", getpid());
    // printf("script_pid is %d\n", script_pid);
    if (getpid() == script_pid)
    {
        //part2;
        yellow_color();
        printf("\nNo foreground process available to send_to_background\n");
        reset_color();
        display_prompt(username_ans, hostname_ans);
        fflush(stdout);

        // part2;
    }
    else
    {
        part2;
        red_color();
        printf("Weird stuff is happening in sigtstp handler\n");
        reset_color();
        fflush(stdout);

        part2;
    }
}
void init_stuff()
{
    signal(SIGCHLD, reapChild);
    signal(SIGINT, sigint_handler);
    signal(SIGTSTP, sigtstp_handler);

    //storing path of new reference HOME in home_dir_path
    get_pwd_path_no_print(home_dir_path);

    //initially curr_dir is also SHELL HOME, also prev_dir_path initialized as home_dir_path
    get_pwd_path_no_print(curr_dir_path);
    strcpy(prev_dir_path, curr_dir_path);

    //storing scipt_pid of the master ./a file
    script_pid = getpid();

    //initially,  number of background commands is zero
    num_jobs_cmd = 0;
    curr_job_id = 1; //this is the lowest available job id usable for assignment
    curr_fg_pid = -1;
    is_legendary = 1;

    //init bg struct
    for (int i = 0; i < jobs_ptr_sz; i++)
    {
        jobs_ptr[i] = (struct jobs_cmd *)malloc(sizeof(struct jobs_cmd));
        jobs_ptr[i]->pid = -2;
        jobs_ptr[i]->jid = -1;
        jobs_ptr[i]->cmd_name[0] = '\0';
        jobs_ptr[i]->cmd_stat = -1;
        jobs_ptr[i]->is_relevant = 0;
    }

    //setup history path
    LL len_home_dir = strlen(home_dir_path);
    for (int i = 0; i < len_home_dir; i++)
    {
        history_file_path[i] = home_dir_path[i];
    }
    history_file_path[len_home_dir] = '\0';
    curr_history_num = 0;
    strcat(history_file_path, "/history.txt\0");

    for (int i = 0; i < 22; i++)
    {
        hist_cmds[i] = (char *)malloc(sizeof(100));
        hist_cmds[i][0] = '\0';
    }
    setup_history();
}

int main()
{
    //LL i;

    init_stuff();
    bool printPrompt = true;

    // char *colon_cmds[50];
    //int colon_cmds_idx = 0;

    get_username_linux(username_ans);
    get_hostname_linux(hostname_ans);

    unsigned long cmd_buffer_sz = 200;
    char *cmd_input = (char *)malloc(sizeof(char) * (cmd_buffer_sz));

    yellow_color();
    printf("Welcome to my shell !!!\n");
    reset_color();

    //printf("pid() of the shell is %d <just before entering while loop>\n", getpid());

    while (true)
    {
        //printf("in loop\n");
        if (printPrompt == false)
        {
            printPrompt = true;
        }
        else
        {
            display_prompt(username_ans, hostname_ans);
        }

        ssize_t chars_read; //stores number of characters read by the getline func

        //&cmd_input needed as in case the buffer gets changed to a different loc, we need it to get updated there
        chars_read = getline(&cmd_input, &cmd_buffer_sz, stdin);
        // printf("pid of process invoking terminal is %d\n", getpid());
        if (chars_read == -1)
        {

            cyan_color();
            printf("EOF detected\n");
            reset_color();
            return 0;
            //printf("WEIRD INPUT ERROR AS first char of input surprisingly had null char\n");
        }
        else
        {
            if (chars_read == 0)
            {
                //i think redundant but still
                continue;
            }

            // if (chars_read == 1 && cmd_input[0] == '\n')
            // {
            //     printPrompt = false;
            // }

            //handling the \n produced due to pressing enter
            cmd_input[chars_read - 1] = '\0';

            //adding command to history
            add_new_cmd(cmd_input);
            send_for_execution(cmd_input);
        }
        part3;
    }

    return 0;
}