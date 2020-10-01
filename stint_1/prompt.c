#include "master_header.h"
#include "global_vars.h"

void red_color()
{
    printf("\033[1;31m");
}

void yellow_color()
{
    printf("\033[1;33m");
}


void err_yellow_color()
{
    fprintf(stderr,"\033[1;33m");
}

void reset_color()
{
    printf("\033[0m");
}

void err_reset_color()
{
    fprintf(stderr,"\033[0m");
}

void cyan_color()
{
    printf("\x1b[33m");
}


//////////////////////////////
void get_username_linux(char *username_str)
{
    //using getuid instead of geteuid-> https://stackoverflow.com/a/14950419/6427607
    uid_t uid = getuid();

    //https://man7.org/linux/man-pages/man3/getpwuid.3p.html
    // char    *pw_name   User's login name.

    // uid = -1;
    // errno=0;
    struct passwd *pw = (struct passwd *)malloc(sizeof(struct passwd));
    pw = getpwuid(uid);

    if (pw != NULL)
    {
        //return pw->pw_name;
        int len = strlen(pw->pw_name);
        strcpy(username_str, pw->pw_name);
        username_str[len] = '\0';
    }
    else
    {
        // perror("Error during username extraction");
        printf("Error occurred extracting username from linux\n");
        exit(1);
    }
}

void get_hostname_linux(char *hostname_ans)
{
    //printf("Username is %s\n", username_ans);
    int check_status_var = gethostname(hostname_ans, 500);
    if (check_status_var == 0)
    {
        //hostname retrieved successfully
        // printf("Hostname is %s\n", hostname_ans);
    }
    else
    {
        perror("Error fetching hostname");
        exit(1);
    }
    /////////////////////////////////////////////////////////////////
}

void display_prompt(char *username_ans,char *hostname_ans)
{
    /////////////////////////////////////////////////////////////////////
    red_color();
    printf("%s@", username_ans);
    printf("%s:", hostname_ans);

    yellow_color();
    set_shell_path();
    printf("%s >", shell_dir_path);

    reset_color();
    ///////////////////////////////////////////////////////////////////////
}
