#ifndef NIGHTSWATCH_HEADER
#define NIGHTSWATCH_HEADER
typedef long long LL;
LL get_recent_pid();
void display_cpu_stuff(int done_already);
void exec_nightswatch(struct cmd_var *ptr);
#endif