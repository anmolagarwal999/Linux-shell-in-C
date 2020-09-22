#include "global_vars.h"
#include "master_header.h"

// struct simple_cmd
// {

//     // Number of arguments
//     int simple_args_num;
//     char *simple_cmd_args[100];
//     char input_file_name[50];
//     char output_file_name[50];
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

void populate_cmd_master(char *input_to_master);

void init_master_cmd_struct(struct master_cmd *ptr);
void init_simple_cmd_struct(struct simple_cmd *ptr);


int separate_piped_cmds(struct master_cmd *ptr);
void debug_simple_cmd(struct simple_cmd *ptr);

void debug_master_cmd(struct master_cmd *ptr);

void parse_cmd_for_master(char *cmd_input, struct master_cmd *ptr);

void init_master_cmd_struct(struct master_cmd *ptr)
{
    ptr->number_of_piped_cmds = 0;
    ptr->master_args_num = 0;
    for(int i=0;i<15;i++)
    {
        init_simple_cmd_struct(&ptr->atomic_cmd[i]);
    }
}

void init_simple_cmd_struct(struct simple_cmd *ptr)
{
    ptr->simple_args_num = 0;
    ptr->input_file_name[0]= '\0';
    ptr->output_file_name[0]= '\0';
    for (int i = 0; i < 100; i++)
    {
        ptr->simple_cmd_args[i] = NULL;
    }
}

void populate_cmd_master(char *input_to_master)
{
    struct master_cmd *curr_master = (struct master_cmd *)malloc(sizeof(struct master_cmd));
    init_master_cmd_struct(curr_master);
    parse_cmd_for_master(input_to_master,curr_master);
    separate_piped_cmds(curr_master);
    debug_master_cmd(curr_master);
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

    for(int i=0;i<ptr->simple_args_num;i++)
    {
        printf("%s\n",ptr->simple_cmd_args[i]);
    }
    part;
}

void debug_master_cmd(struct master_cmd *ptr)
{
    part2;
    printf("number of tokens in master is %d\n", ptr->master_args_num);
    printf("number of piped_cmds in master is %d\n", ptr->number_of_piped_cmds);

    printf("the piped cmds are are \n");

    for(int i=0;i<ptr->number_of_piped_cmds;i++)
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