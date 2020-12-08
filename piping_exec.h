#include "global_vars.h"

#ifndef PIPING_HEADER1
#define PIPING_HEADER1
void populate_cmd_master(char *input_to_master);

void init_master_cmd_struct(struct master_cmd *ptr);
void init_simple_cmd_struct(struct simple_cmd *ptr);


int separate_piped_cmds(struct master_cmd *ptr);
void debug_simple_cmd(struct simple_cmd *ptr);

void debug_master_cmd(struct master_cmd *ptr);

void parse_cmd_for_master(char *cmd_input, struct master_cmd *ptr);
int begin_exec(struct master_cmd *ptr);

#endif
