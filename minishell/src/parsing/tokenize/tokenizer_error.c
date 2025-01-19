/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_error.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 10:34:45 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/19 10:35:19 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	handle_tokenization_error(t_shell_data *shell, int error_flag)
{
	if (error_flag == 1)
		shell->exit_status = 2;
	else if (error_flag == 2)
	{
		ft_putstr_fd("Error: failed to allocate memory\n", STDERR_FILENO);
		cleanup(shell);
		exit(EXIT_FAILURE);
	}
}

void	tokenize_input_error(t_shell_data *shell, int error_flag)
{
	free_commands(shell);
	shell->commands = NULL;
	shell->error_flag = error_flag;
}
