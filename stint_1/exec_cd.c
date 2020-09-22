#include "global_vars.h"
#include "master_header.h"

void exec_cd_cmd(char *cd_input)
{
    int status_cmd;
    char *set_path = cd_input;

    if (cd_input == NULL)
    {
        //go to env $HOME
        
        //char *homedir;
        //https://stackoverflow.com/questions/2910377/get-home-directory-in-linux
        // if ((homedir = getenv("HOME")) == NULL)
        // {
        //     homedir = getpwuid(getuid())->pw_dir;
        // }
        // printf("Address to HOME is %s\n", homedir);
        // // if (cd_input[1] == '\0')
        // // {
        // //     set_path = homedir;
        // // }
        // set_path = homedir;

        //deal with ~/Desktop etc cmds
        //strcpy(set_path,home_dir_path);
        set_path=home_dir_path;

       // LL len_set_path = strlen(set_path);
        // LL len_cd_input = 0;
        // if(cd_input!=NULL)
        // {
        //     len_cd_input = strlen(cd_input);
        // }
        // int i = 0;
        // for (i = 1; i < len_cd_input; i++)
        // {
        //     set_path[len_set_path++] = cd_input[i];
        // }
        //set_path[len_set_path] = '\0';
    }
    else if(cd_input[0] == '~')
    {
        convert_mod_to_abs_path(cd_input);
        set_path=cd_input;
    }
    //printf("FILE WE ARE SEEKING IS %s\n", set_path);
    status_cmd = chdir(set_path);
    if (status_cmd < 0)
    {
        perror("cd error:");
        //part3;
    }
    else
    {
        //THIS IS A EXTERN VARIABLE, check your implementation, TO-DO LIST
        //updating curr_dir
        get_pwd_path(curr_dir_path);
        //printf("CURR DIR IS %s\n",curr_dir_path);
    }
}