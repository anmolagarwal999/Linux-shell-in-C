#include "master_header.h"

typedef long long LL;

void get_pwd_path(char *dir_path)
{
    //https://man7.org/linux/man-pages/man3/getcwd.3.html

    if ((getcwd(dir_path, 1024)) != NULL)
    {
        //printf("Path for curr_dir is %s\n", dir_path);
        printf("%s\n",dir_path);
    }
    else
    {
        perror("Error in getcwd");
    }
}

void get_pwd_path_no_print(char *dir_path)
{
    //https://man7.org/linux/man-pages/man3/getcwd.3.html

    if ((getcwd(dir_path, 1024)) != NULL)
    {
        //printf("Path for curr_dir is %s\n", dir_path);
        //printf("%s\n",dir_path);
    }
    else
    {
        perror("Error in getcwd");
    }
}


void set_shell_path()
{
    //On basis of home_path and curr_path, it formulates shell_path

    //check if shell_home is substring of curr_path
    LL len_home_dir_path = strlen(home_dir_path);
    LL len_path_to_use = strlen(curr_dir_path);
    bool isChild = true;
    if (len_path_to_use < len_home_dir_path)
    {
        isChild = false;
    }
    else
    {
        for (int i = 0; i < len_home_dir_path; i++)
        {
            if (home_dir_path[i] != curr_dir_path[i])
            {
                isChild = false;
            }
        }
    }
    LL len_now = 0;

    if (isChild)
    {
        shell_dir_path[len_now++] = '~';
        for (int i = len_home_dir_path; i < len_path_to_use; i++)
        {
            shell_dir_path[len_now++] = curr_dir_path[i];
        }
        shell_dir_path[len_now] = '\0';
    }
    else
    {
        for (int i = 0; i < len_path_to_use; i++)
        {
            shell_dir_path[len_now++] = curr_dir_path[i];
        }
        shell_dir_path[len_now] = '\0';
    }
}

void convert_abs_to_mod_path(char *path_to_use)
{
    //check if shell_home is substring of curr_path
    LL len_home_dir_path = strlen(home_dir_path);
    LL len_path_to_use = strlen(path_to_use);
    bool isChild = true;
    if (len_path_to_use < len_home_dir_path)
    {
        isChild = false;
    }
    else
    {
        for (int i = 0; i < len_home_dir_path; i++)
        {
            if (home_dir_path[i] != path_to_use[i])
            {
                isChild = false;
            }
        }
    }
    LL len_now = 0;

    if (isChild)
    {
        path_to_use[len_now++] = '~';
        for (int i = len_home_dir_path; i < len_path_to_use; i++)
        {
            path_to_use[len_now++] = path_to_use[i];
        }
        path_to_use[len_now] = '\0';
    }
}

void convert_mod_to_abs_path(char *path_to_use)
{
    //check if shell_home is substring of curr_path

    if (path_to_use != NULL)
    {
        if (path_to_use[0] != '~')
        {
            return;
        }
    }
    LL len_home_dir_path = strlen(home_dir_path);
    LL len_path_to_use = strlen(path_to_use);
    //no slash repetition needed
    LL shift = len_home_dir_path - 1;
    LL final_len = len_path_to_use + shift;
    int i;
    for (i = final_len - 1; i - shift >= 1; i--)
    {
        path_to_use[i] = path_to_use[i - shift];
    }
    for (i = 0; i < len_home_dir_path; i++)
    {
        path_to_use[i] = home_dir_path[i];
    }
    path_to_use[final_len] = '\0';
    // printf("Exited\n");
}