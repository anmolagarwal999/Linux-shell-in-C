#include "global_vars.h"
#include "master_header.h"

void parse_cmd(char *cmd_input, struct cmd_var *ptr);

void execute_cmd(char *cmd_input)
{
    //int parse_stat = 0;
    struct cmd_var cmd;

    parse_cmd(cmd_input, &cmd);
    //printf("Number of args is %d\n", cmd.arg_num);
    if (cmd.arg_num == 0)
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
        int id_cmd = get_cmd_id(cmd.cmd_args[0]);
        // printf("id_cmd is %d\n", id_cmd);
        ////////////////////////////////////////////////////////////////////////////////////
        if (id_cmd == 0)
        {
            //SYSTEM COMMAND
            //run execvp
            //  printf("SYSTEM COMMAND\n");
            handle_system_cmd(&cmd);
        }
        ////////////////////////////////////////////////////////////////////////////////
        else
        {
            //
            if (id_cmd == 1)
            {
                //implement cd
                //IF CD HAS MORE THAN 2 ARGS, it is a disaster
                if (cmd.arg_num > 2)
                {
                    printf("cd:you supplied too many arguments\n");
                }
                else
                {
                    exec_cd_cmd(cmd.cmd_args[1]);
                }
            }
            else if (id_cmd == 2)
            {
                //implement ls
                exec_ls(&cmd);
            }
            else if (id_cmd == 3)
            {
                //pwd
                get_pwd_path(curr_dir_path);
            }
            else if (id_cmd == 4)
            {
                //echo
                exec_echo(&cmd);
            }
            else if (id_cmd == 5)
            {
                //pinfo
                exec_pinfo(&cmd);
            }
            else if (id_cmd == 6)
            {
                int reqd;
                if (cmd.arg_num > 1)
                {
                    reqd = (int)strtol(cmd.cmd_args[1], NULL, 10);
                }
                else
                {
                    reqd = 10;
                }
                display_history(reqd);
            }
            else if(id_cmd==7)
            {
                exit(0);
            }
            else if(id_cmd==8)
            {
                exec_nightswatch(&cmd);
            }
        }
        ///////////////////////////////////////////////////////////////////////
    }
}

int get_cmd_id(char *cmd_name)
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
        id_cmd =7;
    }
    else if (strcmp(cmd_name, "nightswatch") == 0)
    {
        id_cmd =8;
    }
    
    return id_cmd;
}

void parse_cmd(char *cmd_input, struct cmd_var *ptr)
{
    // printf("trying to parse cmd\n");
    // printf("To be parsed is %s\n", cmd_input);

    ptr->arg_num = 0;
    ptr->is_bg = 0;
    //https://man7.org/linux/man-pages/man3/strtok.3.html
    char delims[] = " \t\r\n";
    char *token_beg;

    token_beg = strtok(cmd_input, delims);
    while (token_beg != NULL)
    {
        ptr->cmd_args[ptr->arg_num++] = token_beg;
        token_beg = strtok(NULL, delims);
    }

    //FOR EXECVP,cd-number of args spotting etc, ARG LIST MUST END WITH A NULL CHARACTER
    ptr->cmd_args[ptr->arg_num] = NULL;

    if ((ptr->arg_num > 0) && (strcmp(ptr->cmd_args[ptr->arg_num - 1], "&\0")) == 0)
    {
        //command is background command
        ptr->is_bg = 1;
        ptr->cmd_args[ptr->arg_num - 1] = NULL;
        ptr->arg_num--;
    }
}

void handle_system_cmd(struct cmd_var *ptr)
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