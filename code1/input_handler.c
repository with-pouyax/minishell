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
		shell->input = readline(PROMPT); // using readline we get the input from the user and store it in shell->input
		if (!shell->input) // the shell->input can be NULL if the user presses ctrl+d
			break ;
		if (check_unclosed_quotes(shell->input)) // check if the quotes are not closed
		{
			char	*additional_input; 

			additional_input = readline("quote> "); // we store the additional input in additional_input 
			shell->input = ft_strjoin_free(shell->input, additional_input); // we join the additional input to the original input
		}
		if (ft_strlen(shell->input) > 0) // if the input is not empty
			add_history(shell->input); // we add the input to the history
		tokenize_input(shell->input, shell); // we tokenize the input
		expand_variables_in_tokens(shell);
		parse_tokens(shell);
		print_commands(shell);
		free_commands(shell->commands);
		shell->commands = NULL;
		free(shell->input);
		shell->input = NULL;
	}
}
