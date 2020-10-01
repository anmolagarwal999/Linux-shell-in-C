#ifndef GLOBAL_VARS_HEADER
#define GLOBAL_VARS_HEADER

#include <stdbool.h>
//https://stackoverflow.com/questions/1719662/warning-struct-user-data-s-declared-inside-parameter-list

extern char home_dir_path[1024];  //place from where shell is executed
extern char curr_dir_path[1024];  //curr location in abs loc (improve)
extern char shell_dir_path[1024]; //modifying curr_dir_path for NEW REFERENCE POINT
extern char history_file_path[1024];
extern char prev_dir_path[1024];

//https://stackoverflow.com/questions/53256436/difference-of-pid-t-and-int-in-c
//The pid_t data type is a signed integer type which is capable of representing a process ID.
//In the GNU C Library, this is an int.

//https://stackoverflow.com/questions/1045501/how-do-i-share-variables-between-different-c-files?noredirect=1&lq=1
extern int script_pid;
extern int num_jobs_cmd;
extern int curr_job_id;
extern int curr_history_num;
extern int curr_fg_pid;

#define max_poss_args 100
struct cmd_var
{
    int arg_num;
    char *cmd_args[max_poss_args];
    int is_bg;
};

struct jobs_cmd
{
    int pid,jid;
    char cmd_name[100];
    int cmd_stat;  //running or stopped 1->running, 2 -> stopped
    int is_relevant; // is 1 only if it is not terminated yet, becomes 0 if it has been terminated
};

struct simple_cmd
{

    // Number of arguments
    int simple_args_num;
    char *simple_cmd_args[100];
    char *input_file_name;
    char *output_file_name;
    char *append_file_name;
};

#define jobs_ptr_sz 100
extern struct jobs_cmd *jobs_ptr[jobs_ptr_sz];
extern char *hist_cmds[22];

struct master_cmd
{
    int number_of_piped_cmds;
    int master_args_num;
    char *master_cmd_args[max_poss_args];

    struct simple_cmd atomic_cmd[15];
};
#endif
