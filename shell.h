#ifndef SHELL_H
#define SHELL_H


#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/wait.h>


#define MAX_COMMAND_LENGTH 100
#define PROMPT "simple_shell$ "
#define PATH_DELIMITER ":"

void read_command(char *command, ssize_t size);
void execute_command(char *command);
void tokenize_command(char *command, char **args);
void handle_child_process(char **args);
void handle_parent_process(pid_t pid);


#endif

