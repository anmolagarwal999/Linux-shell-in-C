#include "global_vars.h"
#include "master_header.h"

void exec_echo(struct cmd_var *ptr)
{
    //printf("ECHO STUFF\n");
    for(int i=1;i<ptr->arg_num;i++)
    {
        printf("%s",ptr->cmd_args[i]);
        if(i!=ptr->arg_num-1)
        {
            printf(" ");
        }
    }
    printf("\n");
}