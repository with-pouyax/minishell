/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var_input.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 10:59:01 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/23 15:31:14 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "../../../include/minishell.h"

void	expand_variables_in_input(t_shell_data *shell)
{
	char	*expanded_input;

	expanded_input = expand_variables_in_token(shell, shell->input);
	if (!expanded_input)
	{
		ft_putstr_fd("minishell: memory allocation error\n", STDERR_FILENO);
		free(shell->input);
		shell->input = NULL;
		shell->exit_status = 1;
		return ;
	}
	free(shell->input);
	shell->input = expanded_input;
}
