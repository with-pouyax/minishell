/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 09:42:44 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/19 09:42:45 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../internal_commands.h"

int	ft_export(t_shell_data *shell, t_command *cmd)
{
	t_token	*token;

	token = cmd->token_list->next;
	if (!token)
	{
		print_sorted_env(shell);
		shell->exit_status = 0;
		return (0);
	}
	while (token)
	{
		if (is_valid_identifier(token->value, 1))
			add_to_env(shell, token->value);
		else
		{
			ft_putstr_fd("minishell: export: `", STDERR_FILENO);
			ft_putstr_fd(token->value, STDERR_FILENO);
			ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
			shell->exit_status = 1;
			return (1);
		}
		token = token->next;
	}
	return (0);
}
