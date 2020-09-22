# Assignment 2
#### - 2019101068

### Instructions to run
```
$ make
$ ./a
```
Both **nightswatch** and **history**   have been implemented.

***Typing 'exit' followed by ENTER*** will exit from the shell.

### Files included
* `main.c` -> contains the main loop which takes input from user
* `echo.c` ->  implements echo command
* `exec_cd.c` -> implements 'cd' command
* `execution.c` -> contains functions relating to parsing the string, identifying the command, setting up job control
* `get_history.c` -> contains functions to implement 'history' cmd, reading,writing from history.txt and to know if a command in the shell is eligible to be a part of history (is should not be empty,duplicate or contain just whitespaces)
* `path_related.c`-> implements pwd and other functions to modify *path* keeping in mind the new reference/meaning of HOME
* `ls.c`-> implements ls command (basic usage+ functionality of flags of 'a' and 'l')
* `pinfo.c`-> functions to implement pinfo command
* `reap_child.c`-> deals with child of the parent only if it is 
* `divide_input.c` -> calls the exec_cmd function after accepting atomic commands
* `prompt.c` -> prints the prompt
* `nightswatch.c` -> implements nightswatch command
* `global_vars.h` -> contains definitions of structs etc.
* `master_header.h` -> contains all header files at one place


### Assumptions and other details
- In nightswatch, as mentioned on moodle forum, the loop quits on pressing **q-enter** and not just by pressing 'q'. Also, if option is not ‘newborn’ I assume it to be ‘interrupt’.
- Hostname does not exceed 500 characters
- Input command does not have null character anywhere in between
- I assume that $HOME is set so that cd ~ can work
- File name does not have ‘\0’ anywhere
- Path which the symlink points to(path of executable) does not excess 1e6 characters
- If history command supplies arg which is larger than the number of commands actually stored, then no error is produced. Instead all the commands there in history file are displayed. This actually is what the actual shell does as well and hence, my shell attempts to do the same to ‘mimic shell behavior’.
- Username and hostname aren’t changed in the midst of the program
- Max possible background processes ran in a session of the shell is <=500
- Even though the shell, waits for the next command to get executed before testing bg_process , If i would have implemented in that manner, the time complexity (though will not matter here) would have been more as I would have to iterate through all the processes even though none of them had actually changed their status. Hence, i have tried to catch the changed state via use of SIGCHLD which automatically invokes reapChild whenever an ‘event worth being handled arises’ and hence, helps us to prevent the useless iterations which we would have had to make otherwise.
- Length of an atomic version command ie, after tokenizing wrt semi-colon does not exceed 200
- As mentioned here(https://unix.stackexchange.com/questions/115917/why-is-bash-not-storing-commands-that-start-with-spaces), bash stores commands starting with ‘whitespace’ depending on user settings. For my shell, I am storing commands even if they begin with whitespace but am ignoring commands which just contain whitespace.
- Max number of whitespace-separated tokens in cmd can be 100
- I am assuming that both ‘cd’ and ‘cd ~’ are expected to take user to the newly referenced HOME instead of the usual ‘/home/<user-name>’
- If any of the builtin commands have an ‘&’ as the last token, I am just ignoring the ampersand. Eg: echo ‘abc’ & -> prints ‘abc’
- Assumed pagesize to 4096 and used that to use the factor 1 page=4Kb


