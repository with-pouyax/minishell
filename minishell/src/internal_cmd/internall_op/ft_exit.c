#include "../internal_commands.h"

void	print_exit_error(char *message, int *exit_status, int code)
{
	ft_putstr_fd(message, STDERR_FILENO);
	*exit_status = code;
}

void print_exit_error2(char *format, char *arg, int *exit_status, int code)
{
    fprintf(stderr, format, arg);
    *exit_status = code;
}

/*
** Helper function to replicate strtoll behavior.
** Converts a string to long long while checking for overflows and invalid input.
*/

int	initialize_conversion(const char **str, int *sign)
{
    while (**str == ' ' || **str == '\t' || **str == '\n')
        (*str)++;
    *sign = 1;
    if (**str == '-')
    {
        *sign = -1;
        (*str)++;
    }
    else if (**str == '+')
    {
        *sign = 1;
        (*str)++;
    }
    if (!ft_isdigit(**str))
        return (0);
    return (1);
}

int	parse_number(const char *str, long long *num, int sign)
{
    long long	digit;

    while (ft_isdigit(*str))
    {
        digit = *str - '0';
        if (sign == 1 && (*num > (LLONG_MAX - digit) / 10))
            return (0);
        if (sign == -1 && (-*num < (LLONG_MIN + digit) / 10))
            return (0);
        *num = (*num * 10) + digit;
        str++;
    }
    while (*str == ' ' || *str == '\t' || *str == '\n')
        str++;
    if (*str != '\0')
        return (0);
    return (1);
}

int	str_to_long_long(const char *str, long long *result)
{
    int			sign;
    long long	num;

    num = 0;
    *result = 0;
    if (!initialize_conversion(&str, &sign))
        return (0);
    if (!parse_number(str, &num, sign))
        return (0);
    *result = num * sign;
    return (1);
}

void	cleanup_and_exit(t_shell_data *shell, int exit_status)
{
	cleanup(shell);
	rl_clear_history();
	close(shell->saved_stdin);
	close(shell->saved_stdout);
	exit(exit_status);
}

int	process_exit_arguments(t_shell_data *shell, t_command *cmd, int *exit_status)
{
	t_token	*token;

	token = cmd->token_list->next;
	*exit_status = shell->exit_status;
	if (token)
	{
		if (is_numeric(token->value))
		{
			*exit_status = ft_atoi(token->value) % 256;
			token = token->next;
			if (token)
			{
				print_exit_error("exit: too many arguments\n",
					&shell->exit_status, 1);
				return (1);
			}
		}
		else
		{
			print_exit_error2("exit: %s: numeric argument required\n",
				token->value, exit_status, 2);
		}
	}
	return (0);
}

int	ft_exit_shell(t_shell_data *shell, t_command *cmd)
{
	int	exit_status;
	int	should_not_exit;

	should_not_exit = process_exit_arguments(shell, cmd, &exit_status);
	if (should_not_exit)
		return (1);
	cleanup_and_exit(shell, exit_status);
	return (0); 
}

int	is_numeric(const char *str)
{
	long long num;

	return str_to_long_long(str, &num);
}
