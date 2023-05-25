#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 100
#define PROMPT "simple_shell$ "

void read_command(char *command, ssize_t size);
void execute_command(char *command);
void tokenize_command(char *command, char **args);
void handle_child_process(char **args);
void handle_parent_process(pid_t pid);

int main(void)
{
	char command[MAX_COMMAND_LENGTH];

	while (1)
	{
	write(STDOUT_FILENO, PROMPT, sizeof(PROMPT) - 1);

	read_command(command, sizeof(command));

	execute_command(command);
	}

	return (0);
}

void read_command(char *command, ssize_t size)
{
	ssize_t bytesRead;

	bytesRead = read(STDIN_FILENO, command, size);
	if (bytesRead == -1)
	{
	perror("read");
	exit(1);
	}
	else if (bytesRead == 0)
	{
	write(STDOUT_FILENO, "\n", 1);
	exit(0);
	}

	command[strcspn(command, "\n")] = '\0';
}

void execute_command(char *command)
{
	pid_t pid;

	pid = fork();

	if (pid < 0)
	{
	perror("fork");
	}
	else if (pid == 0)
	{
	char *args[MAX_COMMAND_LENGTH / 2 + 1];

	tokenize_command(command, args);

	handle_child_process(args);

	_exit(0);
	}
	else
	{
	handle_parent_process(pid);
	}
}

void tokenize_command(char *command, char **args)
{
	int i = 0;
	char *token = strtok(command, " ");

	while (token != NULL)
	{
	args[i] = token;
	i++;
	token = strtok(NULL, " ");
	}
	args[i] = NULL;
}

void handle_child_process(char **args)
{
	char *envp[] = {NULL};

	if (access(args[0], X_OK) == 0)
	{
	execve(args[0], args, envp);

	perror("execve");
	}
	else
	{
	write(STDOUT_FILENO, "Command not found or not executable\n", 35);
	}

	_exit(1);
}

void handle_parent_process(pid_t pid)
{
	int status;

	waitpid(pid, &status, 0);
}

