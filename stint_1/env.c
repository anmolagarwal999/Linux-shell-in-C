typedef long long LL;
#include "global_vars.h"
#include "master_header.h"

void exec_set_env_var(struct cmd_var *ptr)
{

    int tot_args = ptr->arg_num;
    if (tot_args - 1 == 0 || tot_args - 1 > 2)
    {
        is_legendary = 0;

        fprintf(stderr, "ERROR in cmd: Invalid number of args provided for SETENV\n");
        return;
    }

    char name_var[30];
    strcpy(name_var, ptr->cmd_args[1]);
    char val_var[30] = "";

    if (tot_args - 1 == 2)
    {
        strcpy(val_var, ptr->cmd_args[tot_args - 1]);
    }

    //https://man7.org/linux/man-pages/man3/setenv.3.html
    //    int setenv(const char *name, const char *value, int overwrite);

    // char *old_val = getenv(name_var);
    // printf("old val is %s\n", old_val);

    int func_stat = setenv(name_var, val_var, 1);

    if (func_stat != 0)
    {
        is_legendary = 0;

        perror("Error during SETENV");
    }

    // char *new_val = getenv(name_var);
    // printf("new val is %s\n", new_val);
}

void exec_unset_env_var(struct cmd_var *ptr)
{

    /*If the named variable does not exist in the current environment,
     the environment shall be unchanged and the function is considered 
     to have completed successfully.*/

    int tot_args = ptr->arg_num;
    if (tot_args - 1 != 1)
    {
        is_legendary = 0;

        fprintf(stderr, "ERROR in cmd: Invalid number of args provided for UNSETENV\n");
        return;
    }

    char name_var[30];
    strcpy(name_var, ptr->cmd_args[1]);

    //    int setenv(const char *name, const char *value, int overwrite);

    //https: //pubs.opengroup.org/onlinepubs/009695399/functions/unsetenv.html

    // char *old_val = getenv(name_var);
    // printf("old val is %s\n", old_val);

    int func_stat = unsetenv(name_var);

    if (func_stat != 0)
    {
        is_legendary = 0;

        perror("Error during UNSETENV");
    }

    // char *new_val = getenv(name_var);
    // printf("new val is %s\n", new_val);
}