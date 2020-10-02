typedef long long LL;
#include "global_vars.h"
#include "master_header.h"

void deal_with_ops(char *cmd_input)
{

    // The return status of AND and OR lists is the exit status of the last command executed in the list.
    int i = 0;
    int len_cmd_input = strlen(cmd_input);
    char lb = 0, ub = 0;
    char *cmd_to_send = (char *)malloc(sizeof(char) * 100);

    for (i = 0; i < len_cmd_input;)
    {
        //debug(i);
        if (cmd_input[i] == '@')
        {
            //printf("\nAND\n");
            int pos_now = 0;

            for (int j = lb; j <= ub; j++)
            {
                cmd_to_send[pos_now++] = cmd_input[j];
            }
            cmd_to_send[pos_now] = '\0';
            is_legendary = 1;

            ////////////////
            //fprintf(stderr, "1 sending %s\n", cmd_to_send);
            execute_command_after_format(cmd_to_send);
            /////////////////////

            if (is_legendary == 1)
            {
                //Don't do anything
                lb = i + 1;
                ub = lb;
                i++;
            }
            else
            {
                //false && echo aaa && echo bbb
                //Don't do anything till next OR is encountered
                int t;
                for (t = i + 1; t < len_cmd_input; t++)
                {
                    if (cmd_input[t] == '$')
                    {
                        break;
                    }
                }

                lb = t + 1;
                if (lb > len_cmd_input)
                {
                    lb = len_cmd_input;
                }
                ub = lb;
                i = lb;
            }
        }
        else if (cmd_input[i] == '$')
        {
            //  printf("\nOR\n");

            int pos_now = 0;

            for (int j = lb; j <= ub; j++)
            {
                cmd_to_send[pos_now++] = cmd_input[j];
            }
            cmd_to_send[pos_now] = '\0';
            is_legendary = 1;

            ////////////////
            //fprintf(stderr, "2 sending %s\n", cmd_to_send);

            execute_command_after_format(cmd_to_send);
            /////////////////////

            if (is_legendary == 1)
            {
                //Don't do anything till next AND is encountered
                int t;
                for (t = i + 1; t < len_cmd_input; t++)
                {
                    if (cmd_input[t] == '@')
                    {
                        break;
                    }
                }

                lb = t + 1;
                if (lb > len_cmd_input)
                {
                    lb = len_cmd_input;
                }
                ub = lb;
                i = lb;
            }
            else
            {
                //Don't do anything

                lb = i + 1;
                ub = lb;
                i++;
            }
        }
        else
        {
            // printf("%c", cmd_input[i]);
            ub = i;
            i++;
        }
    }
    //debug(lb);
    if (lb != len_cmd_input)
    {
        int pos_now = 0;
        // debug(lb);

        for (int j = lb; j <= ub; j++)
        {
            cmd_to_send[pos_now++] = cmd_input[j];
        }
        cmd_to_send[pos_now] = '\0';
        is_legendary = 1;

        ////////////////
       // fprintf(stderr, "3 sending %s\n", cmd_to_send);
        execute_command_after_format(cmd_to_send);
    }

    // printf("\n");

    fflush(stdout);
    free(cmd_to_send);
}

void send_for_execution(char *cmd_input)
{
    //https://man7.org/linux/man-pages/man3/strtok.3.html

    // printf("Attempting to execute %s\n", cmd_input);
    //return;
    char delims[] = ";";
    char *token_beg;

    //setting to zero before next command is read to be divided
    int colon_cmds_idx = 0;
    char *colon_cmds[50];

    colon_cmds[colon_cmds_idx++] = token_beg = strtok(cmd_input, delims);
    while (token_beg != NULL)
    {
        //printf("Token inside loop is %s\n", token_beg);
        colon_cmds[colon_cmds_idx++] = token_beg = strtok(NULL, delims);
    }

    //looping through the individual commands which have been seperated by colons
    char *tmp_str = (char *)malloc(sizeof(char) * 200);
    is_legendary = 1;
    for (int i = 0; i < colon_cmds_idx; i++)
    {
        if (colon_cmds[i] != NULL)
        {
            //func in divide_input.c
            strcpy(tmp_str, colon_cmds[i]);
            deal_with_ops(tmp_str);
        }
    }
    //part2;
}

//Org command format= str1; str2; str3; str4

/*This function received a string which is of str_i format */
void execute_command_after_format(char *cmd_input)
{

    int len_cmd = strlen(cmd_input);
    //  printf("input for execute command after format is %s with len %d\n", cmd_input, len_cmd);

    if (cmd_input[len_cmd - 1] == '\n')
    {
        cmd_input[len_cmd - 1] = '\0';
    }

    //I think this is redundant but still
    convert_mod_to_abs_path(cmd_input);

    unsigned long cmd_buffer_sz = 200;
    // char *cmd_actual = (char *)malloc(sizeof(char) * (cmd_buffer_sz));
    // strcpy(cmd_actual, cmd_input);

    /*making copy so as to not overwrite in str_(i+1) while performing strtok in str_i in the future
    Made sure to have this ont he heap as if on stack, after function will be over, memory will get
     corrupted and may overwrite on other upcoming function's address space*/
    char *input_to_master = (char *)malloc(sizeof(char) * (cmd_buffer_sz));
    strcpy(input_to_master, cmd_input);

    //fprintf(stderr, "Trying to execute %s\n", input_to_master);

    populate_cmd_master(input_to_master);
    free(input_to_master);
    // execute_cmd(cmd_actual);
}