/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by yourname          #+#    #+#             */
/*   Updated: 2024/10/21 15:52:54 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_input(t_shell *shell)
{
	while (1)
	{
		shell->input = readline(PROMPT);
		if (!shell->input)
			break;
		if (check_unclosed_quotes(shell->input))
		{
			char	*additional_input;

			additional_input = readline("quote> ");
			if (!additional_input)
			{
				free(shell->input);
				break;
			}
			shell->input = ft_strjoin_free(shell->input, additional_input);
			if (!shell->input)
			{
				ft_putstr_fd("Error: failed to allocate memory\n", STDERR_FILENO);
				continue;
			}
		}
		if (shell->input && ft_strlen(shell->input) > 0)
			add_history(shell->input);
		shell->commands = NULL;
		tokenize_input(shell->input, shell);
		if (!shell->commands)
		{
			free(shell->input);
			shell->input = NULL;
			continue;
		}
		expand_variables_in_tokens(shell);
		parse_tokens(shell);
		print_commands(shell);
		free_commands(shell->commands);
		shell->commands = NULL;
		free(shell->input);
		shell->input = NULL;
	}
	rl_clear_history();
}


