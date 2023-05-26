/*
 * File: shell_builtin.c
 * Autors:Uchechukwu Okere
 *       WIlliam Obi
 */

#include "shell.h"

int (*get_builtin_func(char *command))(char **args, char **front);
int shellby_exit(char **args, char **front);
int shellby_change_directory(char **args, char __attribute__((__unused__)) **front);
int shellby_display_help(char **args, char __attribute__((__unused__)) **front);

/**
 * get_builtin_func - Matches a command with its corresponding shellby
 *                    builtin function.
 * @command: The command to match.
 *
 * Return: A function pointer to the corresponding builtin function.
 */
int (*get_builtin_func(char *command))(char **args, char **front)
{
	builtin_t funcs[] = {
	{"exit", shellby_exit},
	{"env", shellby_display_env},
	{"setenv", shellby_setenv},
	{"unsetenv", shellby_unsetenv},
	{"cd", shellby_change_directory},
	{"alias", shellby_alias},
	{"help", shellby_display_help},
	{NULL, NULL}
	};
	int i;

	for (i = 0; funcs[i].name != NULL; i++)
	{
	if (_strcmp(funcs[i].name, command) == 0)
	break;
	}

	return (funcs[i].f);
}

/**
 * shellby_exit - Causes the shellby shell to terminate.
 * @args: An array of arguments containing the exit value.
 * @front: A double pointer to the beginning of args.
 *
 * Return: If there are no arguments - -3.
 *         If the given exit value is invalid - 2.
 *         Otherwise - Exits with the given status value.
 *
 * Description: Upon returning -3, the program exits back in the main function.
 */
int shellby_exit(char **args, char **front)
{
	int i, max_val_len = 10;
	unsigned int exit_value = 0, max_exit_val = 1 << (sizeof(int) * 8 - 1);

	if (args[0])
	{
	if (args[0][0] == '+')
	{
	i = 1;
	max_val_len++;
	}

	for (; args[0][i]; i++)
	{
	if (i <= max_val_len && args[0][i] >= '0' && args[0][i] <= '9')
	exit_value = (exit_value * 10) + (args[0][i] - '0');
	else
	return (create_error(--args, 2));
	}
	}
	else
	{
	return (-3);
	}

	if (exit_value > max_exit_val - 1)
	return (create_error(--args, 2));

	args -= 1;
	free_args(args, front);
	free_env();
	free_alias_list(aliases);
	exit(exit_value);
}

/**
 * shellby_change_directory - Changes the current directory of the shellby process.
 * @args: An array of arguments.
 * @front: A double pointer to the beginning of args.
 *
 * Return: If the given string is not a directory - 2.
 *         If an error occurs - -1.
 *         Otherwise - 0.
 */
int shellby_change_directory(char **args, char __attribute__((__unused__)) **front)
{
	char **dir_info, *new_line = "\n";
	char *old_pwd = NULL, *pwd = NULL;
	struct stat dir;

	old_pwd = getcwd(old_pwd, 0);
	if (!old_pwd)
	return (-1);

	if (args[0])
	{
	if (*(args[0]) == '-' || _strcmp(args[0], "--") == 0)
	{
	if ((args[0][1] == '-' && args[0][2] == '\0') ||
	args[0][1] == '\0')
	{
	if (_getenv("OLDPWD") != NULL)
	(chdir(*_getenv("OLDPWD") + 7));
	}
	else
	{
	free(old_pwd);
	return (create_error(args, 2));
	}
	}
	else
	{
	if (stat(args[0], &dir) == 0 && S_ISDIR(dir.st_mode)
	&& ((dir.st_mode & S_IXUSR) != 0))
	chdir(args[0]);
	else
	{
	free(old_pwd);
	return (create_error(args, 2));
	}
	}
	}
	else
	{
	if (_getenv("HOME") != NULL)
	chdir(*(_getenv("HOME")) + 5);
	}

	pwd = getcwd(pwd, 0);
	if (!pwd)
	return (-1);

	dir_info = malloc(sizeof(char *) * 2);
	if (!dir_info)
	return (-1);

	dir_info[0] = "OLDPWD";
	dir_info[1] = old_pwd;
	if (shellby_setenv(dir_info, dir_info) == -1)
	return (-1);

	dir_info[0] = "PWD";
	dir_info[1] = pwd;
	if (shellby_setenv(dir_info, dir_info) == -1)
	return (-1);

	if (args[0] && args[0][0] == '-' && args[0][1] != '-')
	{
	write(STDOUT_FILENO, pwd, _strlen(pwd));
	write(STDOUT_FILENO, new_line, 1);
	}

	free(old_pwd);
	free(pwd);
	free(dir_info);
	return (0);
}

/**
 * shellby_display_help - Displays information about shellby builtin commands.
 * @args: An array of arguments.
 * @front: A pointer to the beginning of args.
 *
 * Return: If an error occurs - -1.
 *         Otherwise - 0.
 */
int shellby_display_help(char **args, char __attribute__((__unused__)) **front)
{
	if (!args[0])
	display_help_all();
	else if (_strcmp(args[0], "alias") == 0)
	display_help_alias();
	else if (_strcmp(args[0], "cd") == 0)
	display_help_cd();
	else if (_strcmp(args[0], "exit") == 0)
	display_help_exit();
	else if (_strcmp(args[0], "env") == 0)
	display_help_env();
	else if (_strcmp(args[0], "setenv") == 0)
	display_help_setenv();
	else if (_strcmp(args[0], "unsetenv") == 0)
	display_help_unsetenv();
	else if (_strcmp(args[0], "help") == 0)
	display_help_help();
	else
	write(STDERR_FILENO, name, _strlen(name));

	return (0);
}

