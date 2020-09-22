#include "master_header.h"
#include "global_vars.h"

//for stat
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

//for file_user and file_group
#include <pwd.h>
#include <grp.h>

void display_obj_info(char *file_path, char *parent_path)
{
    //Describes columns of ls in bash-> https://www.howtoforge.com/linux-ls-command/
    struct stat fileStat;
    LL len1 = strlen(parent_path);
    LL len2 = strlen(file_path);
    char use_path[500];
    int i, now_pos = 0;
    for (i = 0; i < len1; i++)
    {
        use_path[now_pos++] = parent_path[i];
    }
    for (i = 0; i < len2; i++)
    {
        use_path[now_pos++] = file_path[i];
    }
    use_path[now_pos] = '\0';
    // printf("Information for %s\n", use_path);

    if (stat(use_path, &fileStat) < 0)
    {
        perror("some error occurred while using stat %s\n");
        return;
    }

    // The first column shows file permissions
    printf((S_ISDIR(fileStat.st_mode)) ? "d" : "-");
    printf((fileStat.st_mode & S_IRUSR) ? "r" : "-");
    printf((fileStat.st_mode & S_IWUSR) ? "w" : "-");
    printf((fileStat.st_mode & S_IXUSR) ? "x" : "-");
    printf((fileStat.st_mode & S_IRGRP) ? "r" : "-");
    printf((fileStat.st_mode & S_IWGRP) ? "w" : "-");
    printf((fileStat.st_mode & S_IXGRP) ? "x" : "-");
    printf((fileStat.st_mode & S_IROTH) ? "r" : "-");
    printf((fileStat.st_mode & S_IWOTH) ? "w" : "-");
    printf((fileStat.st_mode & S_IXOTH) ? "x" : "-");

    //  The second column shows the number of hard links
    // nlink_t   st_nlink; /* Number of hard links */
    printf("%3ld", (long)fileStat.st_nlink);

    //  The third and the fourth ones are owner and group names
    struct passwd *pwd = getpwuid(fileStat.st_uid);

    if (pwd == NULL)
    {
        perror("\nError while retrieving owner_name of file");
    }
    else
    {
        printf(" %15s", pwd->pw_name);
    }

    struct group *grp = getgrgid(fileStat.st_gid);

    if (grp == NULL)
    {
        perror("\nError while retrieving group_name of file");
    }
    else
    {
        printf(" %15s", grp->gr_name);
    }

    //  fifth is the file size
    printf("%10ld ", fileStat.st_size);

    // Sixth and seventh are date and time of last modification
    //https://man7.org/linux/man-pages/man3/ctime.3.html
    char *time_ans = ctime(&fileStat.st_mtime);
    LL len_time_ans = strlen(time_ans);
    //  printf("\n--time_ns is &  -%s---\n ", time_ans);
    short cnt2 = 0;
    bool got_space = false;

    for (i = 0; i < len_time_ans; i++)
    {
        if (time_ans[i] == ':')
        {
            cnt2++;
            if (cnt2 > 1)
            {
                break;
            }
        }

        if (time_ans[i] == ' ')
        {
            got_space = true;
        }
        if (got_space)
        {
            printf("%c", time_ans[i]);
        }
    }

    //  The last is the name of the file.
    printf("  %s\n", file_path);

    // part;
}

LL get_total_var(char *file_path, char *parent_path)
{
    //Describes columns of ls in bash-> https://www.howtoforge.com/linux-ls-command/
    struct stat fileStat;
    LL len1 = strlen(parent_path);
    LL len2 = strlen(file_path);
    char use_path[500];
    int i, now_pos = 0;
    for (i = 0; i < len1; i++)
    {
        use_path[now_pos++] = parent_path[i];
    }
    for (i = 0; i < len2; i++)
    {
        use_path[now_pos++] = file_path[i];
    }
    use_path[now_pos] = '\0';
    // printf("Information for %s\n", use_path);

    if (stat(use_path, &fileStat) < 0)
    {
        perror("some error occurred while using stat %s\n");
        return -1;
    }
    //blkcnt_t  st_blocks;      /* Number of 512B blocks allocated */

    LL ans = fileStat.st_blocks;
    return ans;
}

void process_dir(char *dir_path, int num_dir_args, bool l_flag, bool a_flag)
{
    //https://unix.stackexchange.com/questions/319359/how-does-linux-calculate-the-total-block-count-when-running-bin-ls-l
    LL tot_var = 0;
    if (dir_path[0] == '\0')
    {
        return;
    }
    //printf("Trying to process dir with path %s\n",dir_path);
    if (num_dir_args != 1)
    {
        //need to print clarification
        printf("%s:\n", dir_path);
    }

    LL len_dir_path = strlen(dir_path);
    // if / not there, appending a '/'
    if (dir_path[len_dir_path - 1] != '/')
    {
        dir_path[len_dir_path] = '/';
        dir_path[len_dir_path + 1] = '\0';
        len_dir_path++;
    }

    struct dirent *curr_dir;
    DIR *dp;
    int done_times = 0;
    for (done_times = 0; done_times < 2; done_times++)
    {
        dp = opendir(dir_path);
        if (dp == NULL)
        {
            perror("ls cannot access this file or directory while using opendir");
            //return -1;
            return;
        }
        //part2;
        while ((curr_dir = readdir(dp)))
        {
            // FOR hidden files/folders stuff-> https://askubuntu.com/questions/107934/how-can-i-make-a-hidden-file-folder

            if (a_flag == false)
            {
                if (curr_dir->d_name[0] == '.')
                {
                    continue;
                }
            }

            if (l_flag == true)
            {
                if (done_times == 0)
                {
                    tot_var += get_total_var(curr_dir->d_name, dir_path);
                }
                else
                {
                    display_obj_info(curr_dir->d_name, dir_path);
                }
            }
            else
            {
                if (done_times == 1)
                {
                    printf("%s\n", curr_dir->d_name);
                }
            }

            // printf("contents is %s\n", (curr_dir->d_name));
        }
        //part2;

        closedir(dp);
        if (done_times == 0)
        {
            if (l_flag == true)
            {
                tot_var /= 2;
                printf("total %lld\n", tot_var);
            }
        }
    }
}

void exec_ls(struct cmd_var *cmd)
{
    bool l_flag = false;
    bool a_flag = false;

   // printf("IT IS LS TIME\n");

    LL num_tokens = cmd->arg_num;
    int i, j;
    LL num_dir_args = 0;

    //starting from 1 as first token will be ls itself
    for (i = 1; i < num_tokens; i++)
    {
        int len_token = strlen(cmd->cmd_args[i]);
        if (cmd->cmd_args[i][0] == '-')
        {
            //flag token
            for (j = 1; j < len_token; j++)
            {
                if (cmd->cmd_args[i][j] == 'l')
                {
                    l_flag = true;
                }
                if (cmd->cmd_args[i][j] == 'a')
                {
                    a_flag = true;
                }
            }
        }
        else
        {
            //definitely a dir argument
            //printf("\nDir we should considered is %s\n", cmd->cmd_args[i]);

            num_dir_args++;
        }
    }
   // printf("num of dir_args is %lld\n", num_dir_args);
    if (num_dir_args == 0)
    {
        num_dir_args = 1;
        process_dir("./", num_dir_args, l_flag, a_flag);
    }
    else
    {
        for (i = 1; i < num_tokens; i++)
        {
            int len_token = strlen(cmd->cmd_args[i]);
            if (cmd->cmd_args[i][0] != '-')
            {
                //not a flag definitely
               // printf("\nDir being considered is %s\n", cmd->cmd_args[i]);

                if (cmd->cmd_args[i][0] == '~')
                {
                    convert_mod_to_abs_path(cmd->cmd_args[i]);
                }
                char *send_dir=(char *)malloc(len_token+1);
                strcpy(send_dir,cmd->cmd_args[i]);
                //printf("\nDir being considered is %s\n", send_dir);
                process_dir(send_dir, num_dir_args, l_flag, a_flag);
                if (num_dir_args > 1)
                {
                    printf("\n");
                }
                free(send_dir);
            }
        }
    }
}