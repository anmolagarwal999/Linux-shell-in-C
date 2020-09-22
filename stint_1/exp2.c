/** Pipeline between three processes.
    http://stackoverflow.com/q/20056084/
   See @chill's answer: http://stackoverflow.com/a/8092270
 */
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h> /* pid_t */
#include <unistd.h>

#define PROGNAME "pipeline-three-processes"

#define Close(FD)                                         \
    do                                                    \
    {                                                     \
        int Close_fd = (FD);                              \
        if (close(Close_fd) == -1)                        \
        {                                                 \
            perror("close");                              \
            fprintf(stderr, "%s:%d: close(" #FD ") %d\n", \
                    __FILE__, __LINE__, Close_fd);        \
        }                                                 \
    } while (0)

static void report_error_and_exit_helper(const char *message, void (*exit_func)(int))
{
    fprintf(stderr, "%s: error: %s (system: %s)\n",
            PROGNAME, message ? message : "", strerror(errno));
    exit_func(EXIT_FAILURE);
}

static void report_error_and_exit(const char *message)
{
    report_error_and_exit_helper(message, exit);
}

static void _report_error_and_exit(const char *message)
{
    report_error_and_exit_helper(message, _exit);
}

/* move oldfd to newfd */
static void redirect(int oldfd, int newfd)
{
    if (oldfd != newfd)
    {
        if (dup2(oldfd, newfd) != -1)
            Close(oldfd); /* successfully redirected */
        else
            _report_error_and_exit("dup2");
    }
}

static void run(char *const argv[], int in, int out)
{
    redirect(in, STDIN_FILENO);   /* <&in  : child reads from in */
    redirect(out, STDOUT_FILENO); /* >&out : child writes to out */

    execvp(argv[0], argv);
    _report_error_and_exit("execvp");
}

int main(void)
{
    const char *ls[] = {"ls", "-l", NULL};
    const char *sort[] = {"sort", "-k9", NULL};
    const char *less[] = {"less", NULL};
    const char **command[] = {ls, sort, less};
    int n = sizeof(command) / sizeof(*command);

    /* run all commands but the last */
    int i = 0;

    //the first command reads from STDIN of CLI
    int in = STDIN_FILENO; /* the first command reads from stdin */

    for (i = 0; i < (n - 1); ++i)
    {
        int fd[2]; /* in/out pipe ends */
        pid_t pid; /* child's pid */

        if (pipe(fd) == -1)
        {
            //error while generating pipe
            report_error_and_exit("pipe");
        }
        else if ((pid = fork()) == -1)
        {
            //error while forking
            report_error_and_exit("fork");
        }
        else if (pid == 0)
        {

            //exclusively in the child
            /* run command[i] in the child process */

            // the 'pipe[0]' ie read end of them pipe is of no use here as child runs cmd 1 in cmd 1 | cmd 2 and fd[0] is read_end for cmd2 and not for cmd 1
            //we are closing this end only in CHILD and not in PARENT

            //'in' is the read end of the pipe | in 'cmd 0 | cmd 1' where this current child is executing cmd-1

            Close(fd[0]);                              /* close unused read end of the pipe */
            run((char *const *)command[i], in, fd[1]); /* FORMAT is as follows ::::  $ command < in > fd[1] */
        }
        else
        { /* parent */


        //////////////ADD WAIT STATEMENT HERE ??????????????????????????????????????????????///

                    ////WAIT/////

        /////////////////////////////////////////////////////////////////////
            assert(pid > 0);

            //necessary to close this otherwise read of cmd_2 will never terminate as it will always see an active 'write_end'
            Close(fd[1]); /* close unused write end of the pipe */

            //in is still set to 'read_end' of pipe | of  'cmd_0 | cmd_1' which is something we no longer need as the command is fg has already been executed as is evident from the wait cmd
            Close(in);    /* close unused read end of the previous pipe */


            in = fd[0];   /* the next command reads from here */
        }
    }
    /* run the last command */
    run((char *const *)command[i], in, STDOUT_FILENO); /* $ command < in */
}