/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 01:02:00 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/21 16:46:15 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/builtins.h"

/*****************************************************************************/
// 🎯 Purpose  :
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷   str1 ->
//     🏷   str2 ->
//
// 🔄 Returns   :  success status
//
/*****************************************************************************/
// 💡 Notes:
//     strspn is a function that returns the number of characters in the
//     initial segment of str1 which consist only of characters from str2
//     so if str1 is "hello" and str2 is "helo", the function will return 4
//     because the initial segment of str1 that consists only of characters
//     from str2 is "hell"
//
/******************************************************************************/
size_t	ft_strspn(const char *str1, const char *str2)
{
	size_t		count;
	const char	*p;

	count = 0;
	while (*str1)
	{
		p = str2;
		while (*p)
		{
			if (*str1 == *p)
				break ;
			p++;
		}
		if (!*p)
			break ;
		str1++;
		count++;
	}
	return (count);
}
/*****************************************************************************/
// 🎯 Purpose  :
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷   value ->
//
// 🔄 Returns   :  success status
//
/*****************************************************************************/
// 💡 Notes:
//     1- if the first character of the value is not '-', it means the value
//        is not a valid n flag, so we return 0
//     2- if the second character of the value is not 'n', it means the value
//        is not a valid n flag, so we return 0
//     3- value + 1 is the address of the second character of the value
//        so we use ft_strspn to check if the rest of the value is only 'n'
//        if the rest of the value is not only 'n', and the length of the rest
//     4- so ft_strspn(value + 1, "n") will return the number of n characters
//        after the first -n, so if the length of the rest of the value is not
//        equal to the number of n characters, it means the value is not a valid
//        n flag, so we return 0
/******************************************************************************/

int	is_valid_n_flag(const char *value)
{
	if (value[0] != '-')
		return (0);
	if (value[1] != 'n')
		return (0);
	if (ft_strspn(value + 1, "n") != strlen(value + 1))
		return (0);
	return (1);
}

/*****************************************************************************/
// 🎯 Purpose  :
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷   shell -> our structor
//     🏷   cmd -> our command
//
// 🔄 Returns   :  success status
//
/*****************************************************************************/
// 💡 Notes:
//     1- we set token to the second token in the cmd linked list, because the
//        first token is the command itself
//     2- we set newline to 1 to print a newline at the end of the echo
//     3- using a while loop, we iterate over the tokens as long as the token
//        is not null and the token is a valid n flag
//        a- if the token is a valid n flag, we set newline to 0
//        b- we move to the next token
//     4- using a while loop, we iterate over the tokens
//        a- we print the value of the token
//        b- if the token has a next token, we print a space
//        c- we move to the next token
//     5- if newline is 1, we print a newline
//     6- if the shell is in interactive mode, meaning the shell is running
//        in the terminal, we print a newline because in the terminal, the
//        cursor is at the end of the line, so we need to print a newline
//        to move the cursor to the next line
//     7- return 0
/******************************************************************************/
int	ft_echo(t_shell_data *shell, t_command *cmd)
{
	t_token	*token;
	int		newline;

	token = cmd->token_list->next;
	newline = 1;
	while (token && is_valid_n_flag(token->value))
	{
		newline = 0;
		token = token->next;
	}
	while (token)
	{
		printf("%s", token->value);
		if (token->next)
			printf(" ");
		token = token->next;
	}
	if (newline)
		printf("\n");
	if (shell->interactive_mode)
		printf("\n");
	return (0);
}
