#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/wait.h>
#include "shell.h"

#define MAX_COMMAND_LENGTH 100
#define PROMPT "simple_shell$ "

/**
 * main - UNIX command line interpreter
 *
 * Return: Always 0.
 */
{
    char command[MAX_COMMAND_LENGTH];
    ssize_t bytesRead;
    pid_t pid;

    while (1)
    {
        /* Display the prompt */
        write(STDOUT_FILENO, PROMPT, sizeof(PROMPT) - 1);

        /* Read the user's input */
        bytesRead = read(STDIN_FILENO, command, sizeof(command));
        if (bytesRead == -1)
        {
            perror("read");
            exit(1);
        }
        else if (bytesRead == 0)
        {
            write(STDOUT_FILENO, "\n", 1);
            break; /* Exit the shell on Ctrl+D (EOF) */
        }

        /* Remove the newline character at the end of the command */
        command[strcspn(command, "\n")] = '\0';

        pid = fork();

        if (pid < 0)
        {
            perror("fork");
        }
        else if (pid == 0)
        {
            /* Child process */
            char *args[MAX_COMMAND_LENGTH / 2 + 1]; /* Command and arguments */
            char *envp[] = {NULL};                   /* Environment variables */
            int i = 0;
            char *token = strtok(command, " ");

            while (token != NULL)
            {
                args[i] = token;
                i++;
                token = strtok(NULL, " ");
            }
            args[i] = NULL;

            if (access(args[0], X_OK) == 0)
            {
                execve(args[0], args, envp);

                /* If execve fails, print an error */
                perror("execve");
            }
            else
            {
                /* If the executable is not found or not executable, print an error */
                write(STDOUT_FILENO, "Command not found or not executable\n", 35);
            }

            _exit(1);
        }
        else
        {
            /* Parent process */
            int status;
            waitpid(pid, &status, 0);
        }
    }

    return (0);
}
