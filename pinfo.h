#ifndef PINFO_HEADER

#define PINFO_HEADER

void exec_pinfo(struct cmd_var *ptr);
char *int_to_string(int query_pid);
long long get_executable_path(char *pid_path, char *buff, int now_sz);
short get_memory(char *pid_path, char *mem_str, char *buff);
short get_status(char *pid_path, char *status_str, char *buff);
short extract_tpgid(char *pid_path, char *tpgid_str, char *buff);

#endif
