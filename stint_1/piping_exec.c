#include "global_vars.h"
#include "master_header.h"

// arg is of form cmd 1 <>
// struct simple_cmd
// {

//     // Number of arguments
//     int simple_args_num;
//     char *simple_cmd_args[100];
//     char *input_file_name;
//     char *output_file_name;
//     char *append_file_name;
// };

// arg is of form cmd1<> | cmd2 <> |cmd3 <>
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

// arg is of form cmd1<> | cmd2 <> |cmd3 <>
void populate_cmd_master(char *input_to_master)
{
    struct master_cmd *curr_master = (struct master_cmd *)malloc(sizeof(struct master_cmd));
    init_master_cmd_struct(curr_master);
    parse_cmd_for_master(input_to_master, curr_master);
    int sep_stat = separate_piped_cmds(curr_master);
    if (sep_stat == -1)
    {
        //some error while breaking tokens as per piping conventions
        return;
    }

    begin_exec(curr_master);
}

int separate_piped_cmds(struct master_cmd *ptr)
{
    int simple_cmd_now_idx = 0;
    int i;

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
                cyan_color();
                fprintf(stderr, "ERROR in cmd format: piping operator has not been succeeded by space\n");
                reset_color();
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

    //number of cmds is expected tobe one more than number of pipes
    ptr->number_of_piped_cmds = simple_cmd_now_idx + 1;
    return 0;
}

void debug_simple_cmd(struct simple_cmd *ptr)
{
    part;
    fprintf(stderr, "number of tokens in simple is %d\n", ptr->simple_args_num);

    fprintf(stderr, "the tokens in piped cmds are \n");

    for (int i = 0; i < ptr->simple_args_num; i++)
    {
        fprintf(stderr, "%s\n", ptr->simple_cmd_args[i]);
    }
    part;
}

void debug_master_cmd(struct master_cmd *ptr)
{
    part2;
    fprintf(stderr, "number of tokens in master is %d\n", ptr->master_args_num);
    fprintf(stderr, "number of piped_cmds in master is %d\n", ptr->number_of_piped_cmds);

    fprintf(stderr, "the piped cmds are are \n");

    for (int i = 0; i < ptr->number_of_piped_cmds; i++)
    {
        debug_simple_cmd(&ptr->atomic_cmd[i]);
    }

    part2;
}

void parse_cmd_for_master(char *cmd_input, struct master_cmd *ptr)
{
    // master_cmd_args is of form s1 s2 s3 | s4 s5 s6 |  s7 s8 ... NULL

    ptr->master_args_num = 0;

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
}

/* move oldfd to newfd */
void redirect(int oldfd, int newfd)
{
    // printf("entered transition\n");
    if (oldfd != newfd)
    {
        // printf("oldfd is %d\n", oldfd);
        // printf("newfd is %d\n", newfd);
        /* Duplicate FD to FD2, closing FD2 and making it open on the same file.  */

        int status_check = dup2(oldfd, newfd);
        // printf("stat_check  is %d\n", status_check);
        // fflush(stdout);

        if (status_check != -1)
            close(oldfd); /* successfully redirected */
        else
            perror("dup2");
    }
    // printf("exited transition\n");
}

void redirect_check(struct simple_cmd *ptr, int *stat_ptr)
{
    int tot = ptr->simple_args_num;
    //int status_ret = 0;
    // fprintf(stderr, "Entered for redirection checking possibility\n");
    for (int i = 0; i < tot; i++)
    {
        if (strcmp(ptr->simple_cmd_args[i], "<") == 0)
        {
            if (i + 1 < tot)
            {
                //char* strcpy(char* destination, const char* source);
                ptr->input_file_name = (char *)malloc(sizeof(char) * 30);
                strcpy(ptr->input_file_name, ptr->simple_cmd_args[i + 1]);
                // fprintf(stderr,"name of input file is %s\n", ptr->input_file_name);
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
                //   fprintf(stderr,"name of output file is %s\n", ptr->output_file_name);
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

void handle_system_cmd_piped(struct cmd_var *ptr)
{
    // pid_t curr_pid = getpid();
    //  fprintf(stderr,"entered the forking function\n");
    fflush(stderr);
    int amBackground = 0;
    if (ptr->is_bg != 0)
    {
        amBackground = 1;
    }

    //printf("CURR PID IS %d\n", curr_pid);
    pid_t child_pid = fork();

    if (ptr->is_bg != 0 && child_pid > 0)
    {
        add_job(child_pid, ptr, 1);
    }

    if (child_pid < 0)
    {
        //printf("ERROR WHILE FORKING\n");
        is_legendary = 0;
        perror("Error during fork() details");
        //exit(1);
    }
    else if (child_pid == 0)
    {

        //background process -> INHERITED->vim inherits signal(SIGTTIN, SIG_IGN);and signal(SIGTTOU, SIG_IGN); which is desirable

        //to do, implement foreground/background tcsetgrp to prevent terminal hogging by bg

        //restoring the cntrl C default behaviour
        //printf("Default restored\n");
        signal(SIGINT, SIG_DFL);
        signal(SIGTSTP, SIG_DFL);

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
            perror("Error details after execvp");
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
            //fprintf(stderr,"in parent\n");
            fflush(stdout);
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
            fflush(stdout);
            int fg_stat_ptr;
            short stat_temp = waitpid(child_pid, &fg_stat_ptr, WUNTRACED);

            fprintf(stderr, "fg_stat after waitpid is is %d\n", fg_stat_ptr);

            if (stat_temp == -1)
            {
                is_legendary = 0;
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

            fprintf(stderr, "Wait over\n");
            curr_fg_pid = -1;

            /* WIFSTOPPED(wstatus)
              returns true if the child process was stopped by delivery of a
              signal; this is possible only if the call was done using WUN‐
              TRACED or when the child is being traced (see ptrace(2)).*/

            if (WIFSTOPPED(fg_stat_ptr))
            {
                is_legendary = 0;
                add_job(child_pid, ptr, 0);
            }
            else if (WIFEXITED(fg_stat_ptr))
            {
                fprintf(stderr, "Process was exited\n");
                if (WEXITSTATUS(fg_stat_ptr) == EXIT_SUCCESS)
                {
                    //legendary is 1
                    fprintf(stderr, "Exit was successful\n");
                }
                else
                {
                    fprintf(stderr, "foreground process exited abnormally->legendary=0\n");
                    is_legendary = 0;
                }
            }
            else
            {
                is_legendary = 0;
            }
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

void execute_cmd_piped(struct cmd_var *ptr)
{
    if (ptr->arg_num == 0)
    {
        //empty command
        //printf("DAMN IT:EMPTY COMMAND\n");
        return;
    }
    else
    {

        if (true)
        {
            //SYSTEM COMMAND
            //run execvp
            //  printf("SYSTEM COMMAND\n");
            handle_system_cmd_piped(ptr);
        }
        ////////////////////////////////////////////////////////////////////////////////
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
    else if (strcmp(cmd_name, "setenv") == 0)
    {
        id_cmd = 9;
    }
    else if (strcmp(cmd_name, "unsetenv") == 0)
    {
        id_cmd = 10;
    }
    else if (strcmp(cmd_name, "fg") == 0)
    {
        id_cmd = 11;
    }
    else if (strcmp(cmd_name, "bg") == 0)
    {
        id_cmd = 12;
    }
    else if (strcmp(cmd_name, "overkill") == 0)
    {
        id_cmd = 13;
    }
    else if (strcmp(cmd_name, "quit") == 0)
    {
        id_cmd = 14;
    }
    // else if (strcmp(cmd_name, "unsetenv") == 0)
    // {
    //     id_cmd = 15;
    // }
    else if (strcmp(cmd_name, "kjob") == 0)
    {
        id_cmd = 16;
    }
    else if (strcmp(cmd_name, "jobs") == 0)
    {
        id_cmd = 17;
    }

    return id_cmd;
}

void exec_simple_cmd(struct simple_cmd *ptr)
{
    //after removal of fork, this process piping needs are set, just treat it as a normal redirection now
    //fd's inherited, pipes, STDIN, STDOUT,

    //currently STDIN and STDOUT of parent also points to the corresponding pipes

    int i;

    // fprintf(stderr,"Executing %s\n", ptr->simple_cmd_args[0]);
    int stat_redirect[3] = {0, 0, 0};
    redirect_check(ptr, stat_redirect);
    int input_from_file = stat_redirect[0];
    int output_to_file = stat_redirect[1];
    int append_to_file = stat_redirect[2];

    /////////////////// CONTAIN PIPES OR ACTUAL CLI' ptr ///////////////////////////
    int preserve_fd_in = dup(STDIN_FILENO);
    int preserve_fd_out = dup(STDOUT_FILENO);

    if (output_to_file == 1 && append_to_file == 1)
    {
        //Multiple redirections, out of scope
        err_yellow_color();
        fprintf(stderr, "Multiple redirections -> unacceptable\n");
        is_legendary = 0;

        err_reset_color();
        goto restoration;
    }

    int proposed_fd_in = -1, proposed_fd_out = -1;
    if (input_from_file == 1)
    {
        if (ptr->input_file_name == NULL)
        {
            is_legendary = 0;

            fprintf(stderr, "No argument supplied for input redirection\n");
            input_from_file = 0;
            goto restoration;
        }
        else
        {
            proposed_fd_in = open(ptr->input_file_name, O_RDONLY);

            if (proposed_fd_in < 0)
            {
                is_legendary = 0;

                perror("Error while opening input file");
                input_from_file = 0;
                goto restoration;
            }

            redirect(proposed_fd_in, STDIN_FILENO);
        }
    }

    if (output_to_file == 1)
    {
        if (ptr->output_file_name == NULL)
        {
            is_legendary = 0;

            fprintf(stderr, "No argument supplied for output redirection\n");
            output_to_file = 0;

            goto restoration;
        }
        else
        {
            proposed_fd_out = open(ptr->output_file_name, O_WRONLY | O_TRUNC | O_CREAT, 0644);
            if (proposed_fd_out < 0)
            {
                is_legendary = 0;

                perror("Error occurred while dealing with output file ");
                output_to_file = 0;

                goto restoration;
            }
            // dup2(proposed_fd_out, STDOUT_FILENO);
            // close(proposed_fd_out);
            // fprintf(stderr,"---proposed_fd_out is %d\n", proposed_fd_out);
            redirect(proposed_fd_out, STDOUT_FILENO);
        }
    }

    if (append_to_file == 1)
    {
        // printf("entered append\n");
        fflush(stdout);
        if (ptr->append_file_name == NULL)
        {
            is_legendary = 0;

            fprintf(stderr, "No argument supplied for append redirection\n");
            append_to_file = 0;
            goto restoration;
        }
        else
        {
            proposed_fd_out = open(ptr->append_file_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (proposed_fd_out < 0)
            {
                is_legendary = 0;

                perror("Error occurred while dealing with APPEND file ");
                append_to_file = 0;
                goto restoration;
            }
            // dup2(proposed_fd_out, STDOUT_FILENO);
            // close(proposed_fd_out);

            redirect(proposed_fd_out, STDOUT_FILENO);
        }
    }

    //  fprintf(stderr,"Dealt with fds\n");
    //fflush(stdout);
    //if either of the inputs, outputs, appends exists, then shift args accordingly
    if ((input_from_file == 1) || (output_to_file == 1) || (append_to_file == 1))
    {
        //shifting of args needed
        //http://teaching.idallen.com/cst8207/13w/notes/200_redirection.html#rule-4-error-messages-do-not-get-redirected-by-default
        /*Shells don’t care where in the command line you put the output file redirection. 
        No matter where in the command line you type it, it has the same effect, though most people put it at the end,*/

        /*The redirection syntax is processed by and removed by the shell before the 
        command runs. The redirection syntax is never counted as arguments to a command.*/

        //format -> s1 s2 > r1 < r2 >> r3 s4 s5 s6

        //fprintf(stderr,"trying to shift\n");

        int tot = ptr->simple_args_num;
        int curr_idx = 0;
        //fprintf(stderr,"tot is %d\n", tot);
        for (int i = 0; i < tot;)
        {
            //fprintf(stderr,"i is %d\n", i);
            if (strcmp(ptr->simple_cmd_args[i], "<") == 0)
            {
                i += 2;
                continue;
            }
            else if (strcmp(ptr->simple_cmd_args[i], ">") == 0)
            {
                i += 2;
                continue;
            }
            else if (strcmp(ptr->simple_cmd_args[i], ">>") == 0)
            {
                i += 2;
                continue;
            }

            if (i >= tot)
            {
                break;
            }
            ptr->simple_cmd_args[curr_idx++] = ptr->simple_cmd_args[i++];
        }
        ptr->simple_args_num = curr_idx;
        // fprintf(stderr,"curr_idx is %d\n", curr_idx);
        ptr->simple_cmd_args[curr_idx] = NULL;
    }
    else
    {
        // fprintf(stderr,"NO REDIRECTION STUFF\n");
    }

    // fprintf(stderr, "args supplied after removal of redirection tokens is %d\n", ptr->simple_args_num);
    // for (i = 0; i < ptr->simple_args_num; i++)
    // {
    //     fprintf(stderr, "%s\n", ptr->simple_cmd_args[i]);
    // }

    //part;
    fflush(stdout);
    //sleep(1);
    //////////////??EXECUTE IT???????????????????????????//

    if (ptr->simple_args_num == 0)
    {
        err_yellow_color();
        is_legendary = 0;

        fprintf(stderr, "ERROR in shell: empty command\n");
        err_reset_color();
        goto restoration;
    }

    ///////////////////////////////////////////////////////////////////////////////
    struct cmd_var *offload_ptr = (struct cmd_var *)malloc(sizeof(struct cmd_var));

    // Resetting is_bg
    offload_ptr->is_bg = 0;
    int tot_args = ptr->simple_args_num;

    if (strcmp(ptr->simple_cmd_args[ptr->simple_args_num - 1], "&\0") == 0)
    {
        //fprintf(stderr, "it is a background job\n");
        offload_ptr->is_bg = 1;

        // & is no longer an argument
        tot_args--;
    }
    offload_ptr->arg_num = tot_args;
    for (i = 0; i < tot_args; i++)
    {
        offload_ptr->cmd_args[i] = ptr->simple_cmd_args[i];
    }

    offload_ptr->cmd_args[tot_args] = NULL;

    ////////////////////////////////////////////////////////////////////

    int id_cmd = get_cmd_id_piped(ptr->simple_cmd_args[0]);
    //fprintf(stderr,"id cmd is %d\n", id_cmd);
    //fflush(stderr);

    if (id_cmd != 0)
    {
        //builtin command

        if (id_cmd == 1)
        {
            //implement cd
            //IF CD HAS MORE THAN 2 ARGS, it is a disaster
            if (offload_ptr->arg_num > 2)
            {
                is_legendary = 0;

                fprintf(stderr, "cd:you supplied too many arguments\n");
            }
            else
            {
                exec_cd_cmd(offload_ptr->cmd_args[1]);
            }
        }
        else if (id_cmd == 2)
        {
            //implement ls
            exec_ls(offload_ptr);
        }
        else if (id_cmd == 3)
        {
            //pwd
            get_pwd_path(curr_dir_path);
        }
        else if (id_cmd == 4)
        {
            //echo
            exec_echo(offload_ptr);
        }
        else if (id_cmd == 5)
        {
            //pinfo
            exec_pinfo(offload_ptr);
        }
        else if (id_cmd == 6)
        {
            int reqd;
            if (offload_ptr->arg_num > 1)
            {
                reqd = (int)strtol(offload_ptr->cmd_args[1], NULL, 10);
            }
            else
            {
                reqd = 10;
            }
            display_history(reqd);
        }
        else if (id_cmd == 7 || id_cmd == 14)
        {
            exit(0);
        }
        else if (id_cmd == 8)
        {
            exec_nightswatch(offload_ptr);
        }
        else if (id_cmd == 9)
        {
            exec_set_env_var(offload_ptr);
        }
        else if (id_cmd == 10)
        {
            exec_unset_env_var(offload_ptr);
        }
        else if (id_cmd == 11)
        {
            exec_fg(offload_ptr);
        }
        else if (id_cmd == 12)
        {
            exec_bg(offload_ptr);
        }
        else if (id_cmd == 13)
        {
            //overkill
            exec_overkill(offload_ptr);
        }
        else if (id_cmd == 14)
        {
            // already included in 7
        }
        else if (id_cmd == 15)
        {
        }
        else if (id_cmd == 16)
        {
            exec_kjob(offload_ptr);
        }
        else if (id_cmd == 17)
        {
            exec_jobs_cmd();
        }
    }
    else
    {
        //system command
        // fprintf(stderr,"Time to invoke system command\n");
        execute_cmd_piped(offload_ptr);
        //  fprintf(stderr,"RETURNED SAFELY\n");
    }

    //execute////////////////////////////////////////////////
restoration:
    if (input_from_file == 1)
    {
        redirect(preserve_fd_in, STDIN_FILENO);
        //close preserve_fd_in
    }
    else
    {
        close(preserve_fd_in);
    }

    if (output_to_file == 1 || append_to_file == 1)
    {
        redirect(preserve_fd_out, STDIN_FILENO);
    }
    else
    {
        close(preserve_fd_out);
    }

    //exit(0);
}

//contains pipe '|' separate commands
int begin_exec(struct master_cmd *ptr)
{
    int n = ptr->number_of_piped_cmds;

    /* run all commands but the last */
    int i = 0;

    int org_std_in = dup(STDIN_FILENO);   //org_in points to shell input
    int org_std_out = dup(STDOUT_FILENO); //org_out points to shell_output

    int pipes_fd[n][2];

    //the first command reads from STDIN of CLI
    //int in_fd = dup(STDIN_FILENO); /* the first command reads from stdin */
    //return;

    for (i = 0; i < n; i++)
    {

        if (i < n - 1)
        {
            //this cmd is not at last, pipe generation needed
            if (pipe(pipes_fd[i]) == -1)
            {
                //error while generating pipe
                perror("pipe");
            }
        }

        if (i == 0)
        {
            // first command

            //read from STDIN

            //pipes fd[0][WRITE_end] points to STDOUT
            dup2(pipes_fd[i][1], STDOUT_FILENO);

            exec_simple_cmd(&ptr->atomic_cmd[i]);

            close(pipes_fd[i][1]);
        }
        else if (i == n - 1)
        {
            //last cmd

            /* Duplicate FD to FD2, closing FD2 and making it open on the same file.  */
            dup2(pipes_fd[i - 1][0], STDIN_FILENO);

            //OUTPUT is definitely STDOUT for the last cmd
            dup2(org_std_out, STDOUT_FILENO);

            //exec command
            exec_simple_cmd(&ptr->atomic_cmd[i]);

            //close pipe's read end,no need to close write end
            //as it has already read, no use of this end anymore
            close(pipes_fd[i - 1][0]);
        }
        else
        {
            //read from prev pipe and send output to curr pipe
            dup2(pipes_fd[i - 1][0], STDIN_FILENO);
            dup2(pipes_fd[i][1], STDOUT_FILENO);

            //exec stuff
            exec_simple_cmd(&ptr->atomic_cmd[i]);

            //closing both in parent
            close(pipes_fd[i - 1][0]);
            close(pipes_fd[i][1]);
        }
    }

    //restoring input , output already restored in last_cmd via backup
    redirect(org_std_in, STDIN_FILENO);
    redirect(org_std_out, STDOUT_FILENO);
    //fprintf(stderr, "originals restored\n");
    fflush(stdout);

    //Close(in_fd);
    // exec_simple_cmd((char *const *)(ptr->atomic_cmd[i].simple_cmd_args), in_fd, STDOUT_FILENO); /* $ command < in */
    return 0;
}
