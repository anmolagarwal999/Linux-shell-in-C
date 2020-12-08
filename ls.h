#ifndef LS_HEADER
#define LS_HEADER

typedef long long LL;

void display_obj_info(char *file_path, char *parent_path);
LL get_total_var(char *file_path, char *parent_path);
void process_dir(char *dir_path, int num_dir_args, bool l_flag, bool a_flag);
void exec_ls(struct cmd_var *cmd);

#endif

