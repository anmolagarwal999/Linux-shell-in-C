#include "global_vars.h"
#include "master_header.h"

// struct simple_cmd
// {

//     // Number of arguments
//     int simple_args_num;
//     char *simple_cmd_args[100];
//     char *input_file_name;
//     char *output_file_name;
//     char *append_file_name;
// };

// struct master_cmd
// {
//     int number_of_piped_cmds;
//     int master_args_num;
//     char *master_cmd_args[max_poss_args];

//     struct simple_cmd atomic_cmd[15];
// };

// struct cmd_var
// {
//     int arg_num;
//     char *cmd_args[max_poss_args];
//     int is_bg;
// };

void init_master_cmd_struct(struct master_cmd *ptr)
{
    ptr->number_of_piped_cmds = 0;
    ptr->master_args_num = 0;
    for (int i = 0; i < 15; i++)
    {
        init_simple_cmd_struct(&ptr->atomic_cmd[i]);
    }
}

void init_simple_cmd_struct(struct simple_cmd *ptr)
{
    ptr->simple_args_num = 0;
    ptr->input_file_name = NULL;
    ptr->output_file_name = NULL;
    ptr->append_file_name = NULL;

    for (int i = 0; i < 100; i++)
    {
        ptr->simple_cmd_args[i] = NULL;
    }
}

void populate_cmd_master(char *input_to_master)
{
    struct master_cmd *curr_master = (struct master_cmd *)malloc(sizeof(struct master_cmd));
    init_master_cmd_struct(curr_master);
    parse_cmd_for_master(input_to_master, curr_master);
    separate_piped_cmds(curr_master);
    debug_master_cmd(curr_master);
    //return;
    begin_exec(curr_master);
}

int separate_piped_cmds(struct master_cmd *ptr)
{
    int simple_cmd_now_idx = 0;
    int i, j, k;

    // master_cmd_args is of form s1 s2 s3 | s4 s5 s6 |  s7 s8 ... NULL
    for (i = 0; i < ptr->master_args_num; i++)
    {
        if (ptr->master_cmd_args[i][0] == '|')
        {
            //piping possible
            if (strlen(ptr->master_cmd_args[i]) != 1)
            {
                //pipe operator is not succeeded by space -> bad omen
                //invalid command
                printf("ERROR in cmd format: piping operator has not been succeeded by space\n");
                return -1;
            }
            else
            {
                //valid piping command
            }
            simple_cmd_now_idx++;
        }
        else
        {
            //argument must be part of simple cmd
            struct simple_cmd *ptr_simple = &(ptr->atomic_cmd[simple_cmd_now_idx]);
            ptr_simple->simple_cmd_args[ptr_simple->simple_args_num++] = ptr->master_cmd_args[i];
        }
    }

    ptr->number_of_piped_cmds = simple_cmd_now_idx + 1;
    return 0;
}

void debug_simple_cmd(struct simple_cmd *ptr)
{
    part;
    printf("number of tokens in simple is %d\n", ptr->simple_args_num);

    printf("the tokens in piped cmds are \n");

    for (int i = 0; i < ptr->simple_args_num; i++)
    {
        printf("%s\n", ptr->simple_cmd_args[i]);
    }
    part;
}

void debug_master_cmd(struct master_cmd *ptr)
{
    part2;
    printf("number of tokens in master is %d\n", ptr->master_args_num);
    printf("number of piped_cmds in master is %d\n", ptr->number_of_piped_cmds);

    printf("the piped cmds are are \n");

    for (int i = 0; i < ptr->number_of_piped_cmds; i++)
    {
        debug_simple_cmd(&ptr->atomic_cmd[i]);
    }

    part2;
}

void parse_cmd_for_master(char *cmd_input, struct master_cmd *ptr)
{
    // printf("trying to parse cmd\n");
    // printf("To be parsed is %s\n", cmd_input);

    ptr->master_args_num = 0;
    //ptr->is_bg = 0;
    //https://man7.org/linux/man-pages/man3/strtok.3.html
    char delims[] = " \t\r\n";
    char *token_beg;

    token_beg = strtok(cmd_input, delims);
    while (token_beg != NULL)
    {
        ptr->master_cmd_args[ptr->master_args_num++] = token_beg;
        token_beg = strtok(NULL, delims);
    }

    //FOR EXECVP,cd-number of args spotting etc, ARG LIST MUST END WITH A NULL CHARACTER
    ptr->master_cmd_args[ptr->master_args_num] = NULL;

    // master_cmd_args is of form s1 s2 s3 | s4 s5 s6 |  s7 s8 ... NULL
}

#define PROGNAME "my_shell_progname"

static void report_error_and_exit_helper(const char *message, void (*exit_func)(int))
{
    fprintf(stderr, "%s: error: %s (system: %s)\n",
            PROGNAME, message ? message : "", strerror(errno));
    exit_func(EXIT_FAILURE);
}

static void report_error_and_exit(const char *message)
{
    report_error_and_exit_helper(message, exit);
}

static void _report_error_and_exit(const char *message)
{
    report_error_and_exit_helper(message, _exit);
}

/* move oldfd to newfd */
void redirect(int oldfd, int newfd)
{
    if (oldfd != newfd)
    {
        if (dup2(oldfd, newfd) != -1)
            close(oldfd); /* successfully redirected */
        else
            _report_error_and_exit("dup2");
    }
}

void redirect_check(struct simple_cmd *ptr, int *stat_ptr)
{
    int tot = ptr->simple_args_num;
    //int status_ret = 0;
    for (int i = 0; i < tot; i++)
    {
        if (strcmp(ptr->simple_cmd_args[i], "<") == 0)
        {
            if (i + 1 < tot)
            {
                //char* strcpy(char* destination, const char* source);
                ptr->input_file_name = (char *)malloc(sizeof(char) * 30);
                strcpy(ptr->input_file_name, ptr->simple_cmd_args[i + 1]);
            }
            stat_ptr[0] = 1;
        }

        if (strcmp(ptr->simple_cmd_args[i], ">") == 0)
        {
            if (i + 1 < tot)
            {
                //char* strcpy(char* destination, const char* source);
                ptr->output_file_name = (char *)malloc(sizeof(char) * 30);
                strcpy(ptr->output_file_name, ptr->simple_cmd_args[i + 1]);
            }
            stat_ptr[1] = 1;
        }

        if (strcmp(ptr->simple_cmd_args[i], ">>") == 0)
        {
            if (i + 1 < tot)
            {
                //char* strcpy(char* destination, const char* source);
                ptr->append_file_name = (char *)malloc(sizeof(char) * 30);
                strcpy(ptr->append_file_name, ptr->simple_cmd_args[i + 1]);
            }
            stat_ptr[2] = 1;
        }
    }
}

void execute_cmd_piped(struct cmd_var *ptr)
{
    //int parse_stat = 0;

    //printf("Number of args is %d\n", cmd.arg_num);
    if (ptr->arg_num == 0)
    {
        //empty command
        //printf("DAMN IT:EMPTY COMMAND\n");
        return;
    }
    else
    {
        //non-zero arguments

        // printf("Actual command is %s\n", cmd.cmd_args[0]);
        //printf("Tokens are \n");
        // for (int i = 0; i < cmd.arg_num; i++)
        // {
        //     printf("%s\n", cmd.cmd_args[i]);
        // }
        int id_cmd = get_cmd_id_piped(ptr->cmd_args[0]);
        // printf("id_cmd is %d\n", id_cmd);
        ////////////////////////////////////////////////////////////////////////////////////
        if (id_cmd == 0)
        {
            //SYSTEM COMMAND
            //run execvp
            //  printf("SYSTEM COMMAND\n");
            handle_system_cmd_piped(ptr);
        }
        ////////////////////////////////////////////////////////////////////////////////
        else
        {
            //
            if (id_cmd == 1)
            {
                //implement cd
                //IF CD HAS MORE THAN 2 ARGS, it is a disaster
                if (ptr->arg_num > 2)
                {
                    printf("cd:you supplied too many arguments\n");
                }
                else
                {
                    exec_cd_cmd(ptr->cmd_args[1]);
                }
            }
            else if (id_cmd == 2)
            {
                //implement ls
                exec_ls(ptr);
            }
            else if (id_cmd == 3)
            {
                //pwd
                get_pwd_path(curr_dir_path);
            }
            else if (id_cmd == 4)
            {
                //echo
                exec_echo(ptr);
            }
            else if (id_cmd == 5)
            {
                //pinfo
                exec_pinfo(ptr);
            }
            else if (id_cmd == 6)
            {
                int reqd;
                if (ptr->arg_num > 1)
                {
                    reqd = (int)strtol(ptr->cmd_args[1], NULL, 10);
                }
                else
                {
                    reqd = 10;
                }
                display_history(reqd);
            }
            else if (id_cmd == 7)
            {
                exit(0);
            }
            else if (id_cmd == 8)
            {
                exec_nightswatch(ptr);
            }
        }
        ///////////////////////////////////////////////////////////////////////
    }
}

int get_cmd_id_piped(char *cmd_name)
{
    //add ls
    //0->execvp allowed
    //1->cd
    //2->ls
    //3->pwd
    //4->echo

    //0->reserved for commands which 'assignment' considers as system commands
    int id_cmd = 0;
    if (strcmp(cmd_name, "cd") == 0)
    {
        id_cmd = 1;
    }
    else if (strcmp(cmd_name, "ls") == 0)
    {
        id_cmd = 2;
    }
    else if (strcmp(cmd_name, "pwd") == 0)
    {
        id_cmd = 3;
    }
    else if (strcmp(cmd_name, "echo") == 0)
    {
        id_cmd = 4;
    }
    else if (strcmp(cmd_name, "pinfo") == 0)
    {
        id_cmd = 5;
    }
    else if (strcmp(cmd_name, "history") == 0)
    {
        id_cmd = 6;
    }
    else if (strcmp(cmd_name, "exit") == 0)
    {
        id_cmd = 7;
    }
    else if (strcmp(cmd_name, "nightswatch") == 0)
    {
        id_cmd = 8;
    }

    return id_cmd;
}

void handle_system_cmd_piped(struct cmd_var *ptr)
{
    // pid_t curr_pid = getpid();
    int amBackground = 0;
    if (ptr->is_bg != 0)
    {
        amBackground = 1;
    }

    //printf("CURR PID IS %d\n", curr_pid);
    pid_t child_pid = fork();

    if (ptr->is_bg != 0 && child_pid > 0)
    {
        //is bg cmd
        bg_ptr[num_bg_cmd]->pid = child_pid;
        bg_ptr[num_bg_cmd]->cmd_name[0] = '\0';
        bg_ptr[num_bg_cmd]->cmd_stat = 1; //process has not yet returned a signal

        for (int i = 0; i < ptr->arg_num; i++)
        {
            strcat(bg_ptr[num_bg_cmd]->cmd_name, ptr->cmd_args[i]);
            strcat(bg_ptr[num_bg_cmd]->cmd_name, " \0");
        }
        num_bg_cmd++;

        printf("Newly created background process with pid : %d\n", child_pid);
    }

    if (child_pid < 0)
    {
        //printf("ERROR WHILE FORKING\n");
        perror("Error during fork() details");
        //exit(1);
    }
    else if (child_pid == 0)
    {

        //background process -> INHERITED->vim inherits signal(SIGTTIN, SIG_IGN);and signal(SIGTTOU, SIG_IGN); which is desirable

        //to do, implement foreground/background tcsetgrp to prevent terminal hogging by bg

        setpgid(getpid(), 0);
        if (amBackground == 0)
        {
            //foreground process, try to get control of terminal
            tcsetpgrp(0, getpid());
            signal(SIGTTIN, SIG_DFL);
            signal(SIGTTOU, SIG_DFL);
        }

        int success_stat = execvp(ptr->cmd_args[0], ptr->cmd_args);
        if (success_stat < 0)
        {
            //printf("ERROR while executing the input command\n");
            perror("Command not found ?\nError details");
            // part;
            // exit(0);
            exit(EXIT_FAILURE);
        }
        exit(EXIT_FAILURE);
    }
    else
    {
        //printf("CHild pid is %d\n", child_pid);

        if (ptr->is_bg == 0)
        {

            //It's Child is FOREGROUND PROCESS

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
            short stat_temp = wait(&child_pid);

            if (stat_temp == -1)
            {
                perror("Error while wait():");
            }

            //Restore control of terminal to self
            tcsetpgrp(0, getpid());

            //Since terminal restores to self,no longer ignore signals
            signal(SIGTTOU, SIG_DFL);
            signal(SIGTTIN, SIG_DFL);
        }
        else
        {
            //It's Child is BACKGROUND PROCESS
            //The SIGCHLD signal is sent to the parent of a child process when it exits, is interrupted, or resumes after being interrupted.

            //setpgid() needed here as well despite already being at fork()==0 as refer as
            //refer to case of ping www.google.com -> this shouldn't stop even user terminates shell and fork()==0 has not yet been entered
            setpgid(child_pid, 0);
            signal(SIGCHLD, reapChild);
        }
    }
}

void exec_simple_cmd(struct simple_cmd *ptr, int expected_in, int expected_out)
{
    int i, j, k;
    redirect(expected_in, STDIN_FILENO);   /* <&in  : child reads from in */
    redirect(expected_out, STDOUT_FILENO); /* >&out : child writes to out */
    printf("Executing %s\n", ptr->simple_cmd_args[0]);
    int stat_redirect[3] = {0, 0, 0};
    redirect_check(ptr, stat_redirect);
    int input_from_file = stat_redirect[0];
    int output_to_file = stat_redirect[1];
    int append_to_file = stat_redirect[2];

    int preserve_fd_in = dup(STDIN_FILENO);
    int preserve_fd_out = dup(STDOUT_FILENO);

    if (output_to_file == 1 && append_to_file == 1)
    {
        //Multiple redirections, out of scope
        printf("Multiple redirections -> out of scope as TA said\n");
        exit(0);
    }

    int proposed_fd_in = -1, proposed_fd_out = -1;
    if (input_from_file == 1)
    {
        if (ptr->input_file_name == NULL)
        {
            printf("No argument supplied for input redirection\n");
            exit(0);
        }
        else
        {
            proposed_fd_in = open(ptr->input_file_name, O_RDONLY);

            if (proposed_fd_in < 0)
            {
                perror("Error while opening input file");
                exit(-1);
            }
            // dup2(proposed_fd_in, STDIN_FILENO);
            // close(proposed_fd_in);

            redirect(proposed_fd_in, STDIN_FILENO);
        }
    }

    if (output_to_file == 1)
    {
        if (ptr->output_file_name == NULL)
        {
            printf("No argument supplied for output redirection\n");
            exit(0);
        }
        else
        {
            proposed_fd_out = open(ptr->output_file_name, O_WRONLY | O_TRUNC | O_CREAT, 0644);
            if (proposed_fd_out < 0)
            {
                perror("Error occurred while dealing with output file ");
                exit(-2);
            }
            // dup2(proposed_fd_out, STDOUT_FILENO);
            // close(proposed_fd_out);

            redirect(proposed_fd_out, STDOUT_FILENO);
        }
    }

    if (append_to_file == 1)
    {
        if (ptr->append_file_name == NULL)
        {
            printf("No argument supplied for append redirection\n");
            exit(0);
        }
        else
        {
            proposed_fd_out = open(ptr->append_file_name, O_WRONLY | O_CREAT, 0644);
            if (proposed_fd_out < 0)
            {
                perror("Error occurred while dealing with APPEND file ");
                exit(-3);
            }
            // dup2(proposed_fd_out, STDOUT_FILENO);
            // close(proposed_fd_out);

            redirect(proposed_fd_out, STDOUT_FILENO);
        }
    }

    //if either of the inputs, outputs, appends exists, then shift args accordingly
    if (input_from_file == 1 || output_to_file == 1 || append_to_file == 1)
    {
        //shifting of args needed
        //http://teaching.idallen.com/cst8207/13w/notes/200_redirection.html#rule-4-error-messages-do-not-get-redirected-by-default
        /*Shells don’t care where in the command line you put the output file redirection. 
        No matter where in the command line you type it, it has the same effect, though most people put it at the end,*/

        /*The redirection syntax is processed by and removed by the shell before the 
        command runs. The redirection syntax is never counted as arguments to a command.*/

        //format -> s1 s2 > r1 < r2 >> r3 s4 s5 s6

        int tot = ptr->simple_args_num;
        int curr_idx = 0;
        for (int i = 0; i < tot; i++)
        {
            if (strcmp(ptr->simple_cmd_args[i], "<") == 0)
            {
                i += 2;
            }
            else if (strcmp(ptr->simple_cmd_args[i], ">") == 0)
            {
                i += 2;
            }
            else if (strcmp(ptr->simple_cmd_args[i], ">>") == 0)
            {
                i += 2;
            }
            ptr->simple_cmd_args[curr_idx++] = ptr->simple_cmd_args[i];
        }
        ptr->simple_args_num = curr_idx;
        ptr->simple_cmd_args[curr_idx] = NULL;
    }

    printf("args supplied after removal of redirection tokens is \n");
    for (i = 0; i < ptr->simple_args_num; i++)
    {
        printf("%s\n", ptr->simple_cmd_args[i]);
    }

    part;
    //////////////??EXECUTE IT???????????????????????????//

    // struct cmd_var
    // {
    //     int arg_num;
    //     char *cmd_args[max_poss_args];
    //     int is_bg;
    // };

    // struct simple_cmd
    // {

    //     // Number of arguments
    //     int simple_args_num;
    //     char *simple_cmd_args[100];
    //     char *input_file_name;
    //     char *output_file_name;
    //     char *append_file_name;
    // };

    struct cmd_var *offload_ptr = (struct cmd_var *)malloc(sizeof(struct cmd_var));

    //checking if bg
    offload_ptr->is_bg = 0;
    int tot_args = ptr->simple_args_num;

    if (strcmp(ptr->simple_cmd_args[ptr->simple_args_num - 1], "&\0") == 0)
    {
        printf("it is a background job\n");
        offload_ptr->is_bg = 1;

        // & is no longer an argument
        tot_args--;
    }
    offload_ptr->arg_num = tot_args;
    for (i = 0; i < tot_args; i++)
    {
        offload_ptr->cmd_args[i]=ptr->simple_cmd_args[i];
    }

    offload_ptr->cmd_args[tot_args]=NULL;

    execute_cmd_piped(offload_ptr);

    //execute////////////////////////////////////////////////

    if (input_from_file == 1)
    {
        dup2(preserve_fd_in, STDIN_FILENO);
        //close preserve_fd_in
    }

    if (output_to_file == 1 || append_to_file == 1)
    {
        dup2(preserve_fd_out, STDIN_FILENO);
    }

    exit(0);

    /*
    execvp(argv[0], argv);
    _report_error_and_exit("execvp");
    */
}

int begin_exec(struct master_cmd *ptr)
{
    int n = ptr->number_of_piped_cmds;

    /* run all commands but the last */
    int i = 0;

    //the first command reads from STDIN of CLI
    int in_fd = dup(STDIN_FILENO); /* the first command reads from stdin */
    //return;

    for (i = 0; i < (n - 1) + 1; i++)
    {
        int fd[2];
        printf("i is %d\n", i);

        if (pipe(fd) == -1)
        {
            //error while generating pipe
            report_error_and_exit("pipe");
        }

        pid_t child_pid = fork();
        if (child_pid == -1)
        {
            //error while forking
            perror("Error during fork() details");
        }
        else if (child_pid == 0)
        {

            //exclusively in the child
            /* run command[i] in the child process */

            // the 'pipe[0]' ie read end of them pipe is of no use here as child runs cmd 1 in cmd 1 | cmd 2 and fd[0] is read_end for cmd2 and not for cmd 1
            //we are closing this end only in CHILD and not in PARENT

            //'in_fd' is the read end of the pipe | in 'cmd_0 | cmd_1' where this current child is executing cmd_1

            /* close unused read end of the pipe */
            if (close(fd[0]) == -1)
            {
                perror("Error in closing fd[0]");
            }

            if (i == n - 1)
            {
                //return ;
                printf("last straw\n");
                exec_simple_cmd(&ptr->atomic_cmd[i], in_fd, STDOUT_FILENO); /* $ command < in */
            }
            else
            {
                exec_simple_cmd(&ptr->atomic_cmd[i], in_fd, fd[1]); /* FORMAT is as follows ::::  $ command < in > fd[1] */
            }
            exit(0);
        }
        else
        { /* parent */

            //////////////ADD WAIT STATEMENT HERE ??????????????????????????????????????????????///

            ////WAIT/////

            /////////////////////////////////////////////////////////////////////
            //assert(pid > 0);

            //necessary to close this otherwise read of cmd_2 will never terminate as it will always see an active 'write_end'
            if (close(fd[1]) == -1)
            {
                perror("Error in closing fd[1]");
            }
            int status_child;
            if ((child_pid = waitpid(child_pid, &status_child, 0)) == -1)
            {
                perror("wait() error");
            }

            //in is still set to 'read_end' of pipe | of  'cmd_0 | cmd_1' which is something we no longer need as the command in
            //fg has already been executed as is evident from the wait cmd

            /* close unused read end of the previous pipe */
            if (close(in_fd) == -1)
            {
                perror("Error in closing in_fd");
            }

            in_fd = fd[0]; /* the next command reads from here */
        }
    }
    /* run the last command */
    //Close(in_fd);
    // exec_simple_cmd((char *const *)(ptr->atomic_cmd[i].simple_cmd_args), in_fd, STDOUT_FILENO); /* $ command < in */
    return 0;
}
