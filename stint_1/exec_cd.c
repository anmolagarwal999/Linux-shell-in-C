#include "global_vars.h"
#include "master_header.h"

void exec_cd_cmd(char *cd_input)
{
    int status_cmd;
    char *set_path = cd_input;
   // printf("entered exec_cd cmd\n");

    if (cd_input == NULL)
    {
        set_path = home_dir_path;
    }
    else if (cd_input[0] == '~')
    {
        convert_mod_to_abs_path(cd_input);
        set_path = cd_input;
    }
    else if(strcmp(cd_input,"-")==0)    
    {
        set_path=prev_dir_path;
    }
    //printf("FILE WE ARE SEEKING IS %s\n", set_path);
    status_cmd = chdir(set_path);
    //printf("status for chdir is %d\n",status_cmd);
    if (status_cmd < 0)
    {
        is_legendary=0;
        perror("chdir error");
        //part3;
    }
    else
    {
        //modify prev path only if curr cd cmd is successful
        //cd is successful

        strcpy(prev_dir_path,curr_dir_path);
        get_pwd_path(curr_dir_path);
       // printf("CURR DIR IS %s\n",curr_dir_path);
    }
}