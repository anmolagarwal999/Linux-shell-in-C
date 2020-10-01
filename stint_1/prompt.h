#ifndef PROMPT_HEADER
#define PROMPT_HEADER

void cyan_color();
void red_color();
void yellow_color();
void reset_color();
void get_username_linux(char *username_str);
void get_hostname_linux(char *hostname_ans);
void display_prompt(char *username_ans,char *hostname_ans);

#endif
