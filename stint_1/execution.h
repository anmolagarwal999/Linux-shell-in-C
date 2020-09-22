#include "global_vars.h"
#ifndef EXECUTION_HEADER


#define EXECUTION_HEADER

void execute_cmd(char *cmd_input);
int get_cmd_id(char *cmd_name);
void handle_system_cmd(struct cmd_var *ptr);

#endif
