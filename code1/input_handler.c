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

void handle_input(t_shell *shell)
{
    while (1) // Infinite loop to keep the shell running and printing the prompt minishell>
    {
        shell->input = readline(PROMPT); // Read the input from the user
        if (!shell->input) // If the input is NULL, break the loop (e.g., Ctrl-D)
            break;
        if (check_unclosed_quotes(shell->input)) // Check if the quotes are closed
        {
            char *additional_input; // Declare a variable to store the additional input

            additional_input = readline("quote> "); // Read the additional input from the user
            shell->input = ft_strjoin_free(shell->input, additional_input); // Join the additional input to the original input
        }
        if (ft_strlen(shell->input) > 0) // If the input is not empty
            add_history(shell->input); // Add the input to the history
        shell->commands = NULL; // Reset commands for new input
        tokenize_input(shell->input, shell); // Tokenize the input into commands and tokens <=>
        expand_variables_in_tokens(shell); 
        parse_tokens(shell);
        print_commands(shell);
        free_commands(shell->commands);
        shell->commands = NULL;
        free(shell->input);
        shell->input = NULL;
    }
}
