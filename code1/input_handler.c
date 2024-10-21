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
			break ;
		if (check_unclosed_quotes(shell->input))
		{
			char	*additional_input;

			additional_input = readline("quote> ");
			shell->input = ft_strjoin_free(shell->input, additional_input);
		}
		if (ft_strlen(shell->input) > 0)
			add_history(shell->input);
		tokenize_input(shell->input, shell);
		expand_variables_in_tokens(shell);
		parse_tokens(shell);
		print_commands(shell);
		free_commands(shell->commands);
		shell->commands = NULL;
		free(shell->input);
		shell->input = NULL;
	}
}
