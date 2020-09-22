CC = gcc
CFLAGS = -Wall -g -c
LDFLAGS =
OBJFILES = main.o execution.o path_related.o exec_cd.o ls.o echo.o pinfo.o reap_child.o get_history.o divide_input.o prompt.o nightswatch.o

TARGET = a

all:$(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) -Wall -g $(OBJFILES) -o $(TARGET) 

main.o: main.c
	$(CC) $(CFLAGS) main.c

execution.o: execution.c
	$(CC) $(CFLAGS) execution.c

path_related.o: path_related.c
	$(CC) $(CFLAGS) path_related.c

exec_cd.o: exec_cd.c
	$(CC) $(CFLAGS) exec_cd.c

ls.o: ls.c
	$(CC) $(CFLAGS) ls.c

echo.o: echo.c
	$(CC) $(CFLAGS) echo.c


pinfo.o: pinfo.c
	$(CC) $(CFLAGS) pinfo.c

reap_child.o: reap_child.c
	$(CC) $(CFLAGS) reap_child.c


get_history.o: get_history.c
	$(CC) $(CFLAGS) get_history.c

divide_input.o: divide_input.c
	$(CC) $(CFLAGS) divide_input.c

prompt.o: prompt.c
	$(CC) $(CFLAGS) prompt.c


nightswatch.o: nightswatch.c
	$(CC) $(CFLAGS) nightswatch.c
	
