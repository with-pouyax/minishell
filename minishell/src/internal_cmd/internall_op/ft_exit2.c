/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 21:49:21 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/23 21:11:01 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

/*****************************************************************************/
//               No explanation needed
/*****************************************************************************/

void	print_exit_error(char *message, int *exit_status, int code)
{
	ft_putstr_fd(message, STDERR_FILENO);
	*exit_status = code;
}
/*****************************************************************************/
//               No explanation needed
/*****************************************************************************/


void	print_exit_error2(char *format, char *arg, int *exit_status, int code)
{
	(void)format; // Since we're not using the format string, we can safely ignore it
	ft_putstr_fd("exit: ", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
	*exit_status = code;
}


/*****************************************************************************/
// 🎯 Purpose  :
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷 str
//
// 🔄 Returns   : success status
//
/*****************************************************************************/
// 💡 Notes:
//     1- here we convert the string to a long long number and store it in the
//        result. we do it because the exit status should be an integer.
//
/******************************************************************************/

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

/*****************************************************************************/
// 🎯 Purpose  :
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷
//
// 🔄 Returns   : success status
//
/*****************************************************************************/
// 💡 Notes:
//     1- in this function, we convert the string to a long long number and
//        check for overflows and invalid input and store the result in num
//    2- if the number is greater than LLONG_MAX - digit / 10, we return 0
//    3- if the number is less than LLONG_MIN + digit / 10, we return 0
//    4- we multiply the number by 10 and add the digit to it
//    5- we skip the spaces and tabs
//    6- if the string is not empty, we return 0
//    7- we return 1
//
/******************************************************************************/

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