#ifndef MASTER_HEADER
#define MASTER_HEADER

#include <stdio.h>
#include <pwd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <unistd.h> //for hostname
#include <string.h> //for strtok
#include <sys/types.h>
#include <fcntl.h>

#include "execution.h"
#include "path_related.h"
#include "exec_cd.h"
#include "ls.h"
#include "echo.h"
#include "pinfo.h"
#include "reap_child.h"
#include "get_history.h"
#include "divide_input.h"
#include "prompt.h"
#include "nightswatch.h"
#include "piping_exec.h"
#include "env.h"

#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>

typedef long long LL;
#define part fprintf(stderr,"-----------------------------------------\n");
#define part2 fprintf(stderr,"====================================================\n");
#define part3 fprintf(stderr,"############################################################\n");

#endif