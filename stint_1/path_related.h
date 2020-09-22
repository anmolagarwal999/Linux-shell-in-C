#ifndef PATH_RELATED_HEADER
#define PATH_RELATED_HEADER

void get_pwd_path(char *home_dir_path);
void get_pwd_path_no_print(char *dir_path);
void set_shell_path();
void convert_abs_to_mod_path(char *path_to_use);
void convert_mod_to_abs_path(char *path_to_use);

#endif