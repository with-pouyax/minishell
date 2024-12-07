#include "../internal_commands.h"
#include <errno.h>
#include <limits.h>

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
int	str_to_long_long(const char *str, long long *result)
{
	long long	num = 0;
	int			sign = 1;

	*result = 0;

	// Skip leading whitespaces
	while (*str == ' ' || *str == '\t' || *str == '\n')
		str++;

	// Handle sign
	if (*str == '-' || *str == '+')
	{
		sign = (*str == '-') ? -1 : 1;
		str++;
	}

	// Ensure valid input
	if (!(*str >= '0' && *str <= '9'))
		return (0);

	// Parse digits
	while (*str >= '0' && *str <= '9')
	{
		int digit = *str - '0';

		// Check for overflow/underflow
		if (sign == 1 && (num > (LLONG_MAX - digit) / 10))
			return (0);
		if (sign == -1 && (-num < (LLONG_MIN + digit) / 10))
			return (0);

		num = num * 10 + digit;
		str++;
	}

	// Skip trailing whitespaces
	while (*str == ' ' || *str == '\t' || *str == '\n')
		str++;

	// Ensure no extra characters
	if (*str != '\0')
		return (0);

	*result = num * sign;
	return (1);
}

/*
** Exit function
*/
int	ft_exit_shell(t_shell_data *shell, t_command *cmd)
{
	t_token	*token;
	int		exit_status;
	// long long num;

	token = cmd->token_list->next;
	exit_status = shell->exit_status;

	if (token)
	{
		if (is_numeric(token->value))
		{
			exit_status = ft_atoi(token->value) % 256;
			token = token->next;
			if (token)
			{
				print_exit_error("exit: too many arguments\n", &shell->exit_status, 1);
				return (1);
			}
        }
		else
            print_exit_error2("exit: %s: numeric argument required\n", token->value, &exit_status, 255);
    }
	cleanup(shell);
	rl_clear_history();
	exit(exit_status);
}

/*
** Updated is_numeric to use the helper function
*/
int	is_numeric(const char *str)
{
	long long num;

	// Use helper function to validate and convert number
	return str_to_long_long(str, &num);
}
