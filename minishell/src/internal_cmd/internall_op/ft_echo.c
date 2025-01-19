/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 01:02:00 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/19 01:06:29 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../internal_commands.h"

//compute the length of the initial segment of a string
// that consists entirely of characters from a specified set.
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

/*
while (token && ft_strcmp(token->value, "-n") == 0)-> Handle multiple -n options
while (token)                                      -> Print arguments
Only print the newline if we're in interactive mode
		if (newline && shell->interactive_mode)

*/
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
