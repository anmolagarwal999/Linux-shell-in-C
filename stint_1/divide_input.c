typedef long long LL;
#include "global_vars.h"
#include "master_header.h"

void execute_command_after_format(char *cmd_input)
{
    LL len_cmd = strlen(cmd_input);
    if (cmd_input[len_cmd - 1] == '\n')
    {
        cmd_input[len_cmd - 1] = '\0';
    }
    convert_mod_to_abs_path(cmd_input);
    //printf("after conversion is %s\n", cmd_input);
    len_cmd--;
    // printf("Plans to enter execution\n");
    unsigned long cmd_buffer_sz = 200;
    char *cmd_actual = (char *)malloc(sizeof(char) * (cmd_buffer_sz));
    strcpy(cmd_actual, cmd_input);
    //printf("New formation to %s\n", cmd_actual);

    execute_cmd(cmd_actual);
}