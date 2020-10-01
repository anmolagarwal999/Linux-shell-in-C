typedef long long LL;
#include "global_vars.h"
#include "master_header.h"

//Org command format= str1; str2; str3; str4

/*This function received a string which is of str_i format */
void execute_command_after_format(char *cmd_input)
{

    int len_cmd = strlen(cmd_input);
    printf("input for execute command after format is %s with len %d\n", cmd_input, len_cmd);

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

    populate_cmd_master(input_to_master);

    // execute_cmd(cmd_actual);
}