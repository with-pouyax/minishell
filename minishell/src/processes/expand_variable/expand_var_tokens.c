#include "../../minishell.h"

/*****************************************************************************/
// 🎯 Purpose  :
/*****************************************************************************/
//
// 🔹 Parameters:
//	   🏷  result ->  result is a pointer to a string that we store
//         the result in it
//     🏷  in_single_quote -> a pointer to an integer that we store the
//         single quote flag in it
//     🏷  in_double_quote -> a pointer to an integer that we store the
//         double quote flag in it
//
// 🔄 Returns   :  success status
//
/*****************************************************************************/
// 💡 Notes:
//     1- we allocate memory for the result and set it to an empty string.
//     2- if there is an error we print an error message and return 1.
//     3- we set the quote flags to 0.
//     4- we return 0.
//
/******************************************************************************/

int initialize_expansion(char **result, int *in_single_quote, \
int *in_double_quote)
{
    *in_single_quote = 0;
    *in_double_quote = 0;
    *result = ft_strdup("");
    if (!*result)
	{
		ft_putstr_fd("minishell: memory allocation error\n", STDERR_FILENO);
        return (1);
	}
    return (0);
}

void toggle_quotes_and_skip(char current_char, int *in_single_quote, \
int *in_double_quote, int *i)
{
    if (current_char == '\'' && !(*in_double_quote))
        *in_single_quote = !(*in_single_quote);
    else if (current_char == '\"' && !(*in_single_quote))
        *in_double_quote = !(*in_double_quote);
    (*i)++;
}


int cleanup_and_return_null(char *result)
{
    free(result);
    return (1);
}

static int	append_str_to_result(char **result, char *str)
{
	char	*temp;

	(void)str;
	temp = ft_strjoin_free_both(*result, str);
	if (!temp)
	{
		ft_putstr_fd("minishell: memory allocation error\n", STDERR_FILENO);
		return (1);
	}
	*result = temp;
	return (0);
}

static int	handle_exit_status(t_shell_data *shell, char **result)
{
	char	*exit_str;

	(void)shell;
	exit_str = ft_itoa(shell->exit_status);
	if (!exit_str)
	{
		ft_putstr_fd("minishell: memory allocation error\n", STDERR_FILENO);
		return (1);
	}
	if (append_str_to_result(result, exit_str))
		return (1);
	return (0);
}

static int	handle_variable_expansion(t_shell_data *shell, char *input, int *i, char **result)
{
	char	*var_value;
	int		var_not_found_flag;

	var_not_found_flag = 0;
	var_value = get_variable_value(shell, input, i, &var_not_found_flag);
	if (!var_value)
		return (1);
	if (append_str_to_result(result, var_value))
		return (1);
	return (0);
}


int	handle_question_mark(t_shell_data *shell, char **result, int *i)
{
	(*i)++;
	if (handle_exit_status(shell, result))
		return (1);
	return (0);
}

int	handle_alpha_or_underscore(t_shell_data *shell, char *input, int *i, char **result)
{
	if (handle_variable_expansion(shell, input, i, result))
		return (1);
	return (0);
}

void	handle_digit(char *input, int *i)
{
	(void) input;
	(*i)++;
}

int	handle_default_case(char **result)
{
	char	*temp;

	temp = ft_strdup("$");
	if (!temp)
	{
		ft_putstr_fd("minishell: memory allocation error\n", STDERR_FILENO);
		return (1);
	}
	if (append_str_to_result(result, temp))
	{
		free(temp);
		return (1);
	}
	return (0);
}

int	handle_dollar(t_shell_data *shell, char *input, int *i, char **result)
{
	(*i)++;
	if (input[*i] == '?')
	{
		if (handle_question_mark(shell, result, i))
			return (1);
	}
	else if (ft_isalpha((unsigned char)input[*i]) || input[*i] == '_')
	{
		if (handle_alpha_or_underscore(shell, input, i, result))
			return (1);
	}
	else if (ft_isdigit((unsigned char)input[*i]))
	{
		handle_digit(input, i);
	}
	else
	{
		if (handle_default_case(result))
			return (1);
	}
	return (0);
}



void	handle_quote(char current_char, int *in_single_quote, \
							int *in_double_quote, int *i)
{
	toggle_quotes_and_skip(current_char, in_single_quote, in_double_quote, i);
}

int	handle_dollar_expansion(t_shell_data *shell, char *input, \
									int *i, char **result, int in_double_quote)
{
	shell->double_quoted = in_double_quote;
	shell->expanded = 1;
	if (handle_dollar(shell, input, i, result))
	{
		free(*result);
		return (1);
	}
	return (0);
}

int	handle_literal_character(char *input, int *i, char **result)
{
	if (append_literal_char(input, i, result))
	{
		free(*result);
		return (1);
	}
	return (0);
}

int	process_char(t_shell_data *shell, char *input, \
int *i, char **result, int *in_single_quote, int *in_double_quote)
{
	char	current_char;

	current_char = input[*i];
	if ((current_char == '\'' && !(*in_double_quote)) || \
		(current_char == '\"' && !(*in_single_quote)))
	{
		handle_quote(current_char, in_single_quote, in_double_quote, i);
	}
	else if (current_char == '$' && !(*in_single_quote))
	{
		if (handle_dollar_expansion(shell, input, i, result, \
									*in_double_quote))
			return (1);
	}
	else
	{
		if (handle_literal_character(input, i, result))
			return (1);
	}
	return (0);
}

/*****************************************************************************/
//         No explanation needed for this function
/*****************************************************************************/

static int	initialize_and_check(char **result, int *in_single_quote, \
									int *in_double_quote)
{
	if (initialize_expansion(result, in_single_quote, in_double_quote))
		return (1);
	return (0);
}

/*****************************************************************************/
// 🎯 Purpose  :
/*****************************************************************************/
//
// 🔹 Parameters:
//	   🏷  shell -> our structure
//     🏷  input -> user input
//
// 🔄 Returns   :  success status
//
/*****************************************************************************/
// 💡 Notes:
//     1- using initialize_and_check() we allocate memory for the result and
//        set the quote flags to 0.
//        a- if there is an error we return NULL.
//     2- using a while loop we iterate over the input characters
//        a- using process_char() we process the current character.
//
/******************************************************************************/

char	*expand_variables_in_token(t_shell_data *shell, char *input)
{
	char	*result;
	int		i;
	int		in_single_quote;
	int		in_double_quote;

	if (initialize_and_check(&result, &in_single_quote, &in_double_quote))
		return (NULL);
	i = 0;
	while (input[i])
	{
		if (process_char(shell, input, &i, &result, \
								&in_single_quote, &in_double_quote))
			return (NULL);
	}
	return (result);
}

