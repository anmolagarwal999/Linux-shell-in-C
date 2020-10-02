#include "global_vars.h"
#include "master_header.h"

void setup_history()
{
    //check if file exists
    int fd = open(history_file_path, O_RDWR | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);

    // char hist_buff[5001];
    char *hist_buff=(char *)malloc(5001);
    int file_len = read(fd, hist_buff, 5000);
    //printf("file_len is %d\n",file_len);
    hist_buff[file_len]='\0';

    char delims[] = "\n";
    char *token_beg;
    int i = 0;

    token_beg = strtok(hist_buff, delims);
    while (token_beg != NULL)
    {
        hist_cmds[i++] = token_beg;
        token_beg = strtok(NULL, delims);
    }
    curr_history_num = i;

    // printf("interpretations are \n");
    // for(int i=0;i<curr_history_num;i++)
    // {
    //     printf("i is %s\n",hist_cmds[i]);
    // }
    // printf("curr history num is %d\n", curr_history_num);
    close(fd);
}

void rewrite_history()
{
    int fd = open(history_file_path, O_RDWR | O_CREAT | O_TRUNC, S_IRWXU | S_IRWXG | S_IRWXO);

    for (int i = 0; i < curr_history_num; i++)
    {
        int len_cmd = strlen(hist_cmds[i]);
        int bytes_written = write(fd, hist_cmds[i], len_cmd);
        // printf("i printed %s\n",hist_cmds[i]);

        if (bytes_written != len_cmd)
        {
            fprintf(stderr,"\nError while writing to the history file\n");
            //perror("Error details");
            return;
        }
        write(fd, "\n", 1);
    }
    close(fd);
}

void add_new_cmd(char *cmd_to_add)
{
    // if max_limit not violated, just add new command
    //printf("Addition of new command sequence initiated\n");
    if (cmd_to_add[0] == '\0')
    {
        return;
    }

    //also, eliminate commands which contain just spaces
    //https://unix.stackexchange.com/questions/115917/why-is-bash-not-storing-commands-that-start-with-spaces
    LL len=strlen(cmd_to_add);
    int others=0;
    for(int i=0;i<len;i++)
    {
        char ch=cmd_to_add[i];
        if(!(ch==' ' || ch=='\t' || ch=='\n'))
        {
            others++;
            break;
        }
    }
    if(others==0)
    {
        return;
    }
    //////////////////////////////////////////////////////////////////

    if (curr_history_num > 0)
    {
        if ((strcmp(cmd_to_add, hist_cmds[curr_history_num - 1])) == 0)
        {
            return;
        }
    }

    if (curr_history_num >= 20)
    {
       // printf("Hist commands exceeding 20\n");
       // char *tmp_ptr=hist_cmds[0];
        //delete the command which was entered the last
        for (int i = 0; i < curr_history_num - 1; i++)
        {
            hist_cmds[i] = hist_cmds[i + 1];
            //printf("i will now contain %s\n",hist_cmds[i]);
        }
        curr_history_num--;
        //hist_cmds[curr_history_num]=tmp_ptr;
        hist_cmds[curr_history_num]=(char *)malloc(100);
        strcpy(hist_cmds[curr_history_num], cmd_to_add);
    }
    else
    {
        strcpy(hist_cmds[curr_history_num], cmd_to_add);
    }

    curr_history_num++;

    // for (int i = 0; i < curr_history_num; i++)
    // {
    //     printf("i will now contain %s\n", hist_cmds[i]);
    // }

    //you do not know when the user may terminate, hence keep updating
    rewrite_history();
}

void display_history(int needed)
{
    if(needed>curr_history_num)
    {
        needed=curr_history_num;
    }
    for (int i = curr_history_num-needed; i < curr_history_num; i++)
    {
        printf("%s\n", hist_cmds[i]);
    }
}