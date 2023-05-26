/*
 * File: shellby_builtin.c
 * Authors: Uchechukwu Okere
 *       Willian Obi
 *
 * Description: Implementation of built-in commands for the shell.
 */

#include "shell.h"

int shellby_builtin_alias(char **args, char __attribute__ **front);
void shellby_set_alias(char *alias_name, char *alias_value);
void shellby_print_alias(alias_t *alias);
char **shellby_replace_aliases(char **args);

/**
 * shellby_builtin_alias - Built-in command that either
 *  lists all aliases, shows
 * specific aliases, or sets an alias.
 * @args: Array of arguments.
 * @front: Double pointer to the beginning of args.
 *
 * Return: 0 on success, -1 on error.
 */
int shellby_builtin_alias(char **args, char __attribute__ **front)
{
	alias_t *temp = aliases;
	int i, ret = 0;
	char *value;

	if (!args[0])
	{
		/* List all aliases */
		while (temp)
		{
			shellby_print_alias(temp);
			temp = temp->next;
		}
		return (ret);
	}

	for (i = 0; args[i]; i++)
	{
		temp = aliases;
		value = _strchr(args[i], '=');
		if (!value)
		{
			/* Show specific alias */
			while (temp)
			{
				if (_strcmp(args[i], temp->name) == 0)
				{
					shellby_print_alias(temp);
					break;
				}
				temp = temp->next;
			}
			if (!temp)
				ret = shellby_create_error(args + i, 1);
		}
		else
			shellby_set_alias(args[i], value);
	}

	return (ret);
}

/**
 * shellby_set_alias - Sets an existing alias with a
 * new value or creates a new
 * alias with the given name and value.
 * @alias_name: Name of the alias.
 * @alias_value: Value of the alias (including '=').
 */
void shellby_set_alias(char *alias_name, char *alias_value)
{
	alias_t *temp = aliases;
	int value_len, j, k;
	char *new_value;

	*alias_value = '\0';
	alias_value++;
	value_len = _strlen(alias_value) - _strspn(alias_value, "'\"");
	new_value = malloc(sizeof(char) * (value_len + 1));
	if (!new_value)
		return;
	for (j = 0, k = 0; alias_value[j]; j++)
	{
		if (alias_value[j] != '\'' && alias_value[j] != '"')
			new_value[k++] = alias_value[j];
	}
	new_value[k] = '\0';
	while (temp)
	{
		if (_strcmp(alias_name, temp->name) == 0)
		{
			free(temp->value);
			temp->value = new_value;
			break;
		}
		temp = temp->next;
	}
	if (!temp)
		shellby_add_alias_end(&aliases, alias_name, new_value);
}

/**
 * shellby_print_alias - Prints an alias in the format 'name=value'.
 * @alias: Pointer to an alias.
 */
void shellby_print_alias(alias_t *alias)
{
	char *alias_string;
	int string_len = _strlen(alias->name) + _strlen(alias->value) + 2;

	alias_string = malloc(sizeof(char) * (string_len + 1));
	if (!alias_string)
		return;
	_strcpy(alias_string, alias->name);
	_strcat(alias_string, "=");
	_strcat(alias_string, alias->value);
	_strcat(alias_string, "\n");

	write(STDOUT_FILENO, alias_string, string_len);
	free(alias_string);
}

/**
 * shellby_replace_aliases - Replaces any matching aliases in
 * the arguments with their respective values.
 * @args: Array of arguments.
 *
 * Return: Updated array of arguments.
 */
char **shellby_replace_aliases(char **args)
{
	alias_t *temp;
	int i;
	char *new_value;

	if (_strcmp(args[0], "alias") == 0)
		return (args);

	for (i = 0; args[i]; i++)
	{
		temp = aliases;
		while (temp)
		{
			if (_strcmp(args[i], temp->name) == 0)
			{
				new_value = malloc(sizeof(char) * (_strlen(temp->value) + 1));
				if (!new_value)
				{
					shellby_free_args(args, args);
					return (NULL);
				}
				_strcpy(new_value, temp->value);
				free(args[i]);
				args[i] = new_value;
				i--;
				break;
			}
			temp = temp->next;
		}
	}

	return (args);
}

