#include "../minishell.h"

int	free_and_return(char *str)
{
	free(str);
	return (1);
}

void	free_envp(t_shell_data *shell)
{
	int	i;

	i = 0;
	if (!shell->envp)
		return ;
	while (shell->envp[i])
	{
		free(shell->envp[i]);
		i++;
	}
	free(shell->envp);
	shell->envp = NULL;
}

void	cleanup(t_shell_data *shell)
{
	free(shell->input); 
	shell->input = NULL;
	free(shell->full_input);
	shell->full_input = NULL;
	free_commands(shell);
	free_envp(shell);
	if (shell->prev_dir) {
        free(shell->prev_dir);
        shell->prev_dir = NULL;
    }
}

int tokenize_command_error(t_command *cmd)
{
    // Free tokens
    if (cmd->token_list)
    {
        free_tokens(cmd->token_list);
        cmd->token_list = NULL;
    }

    // Free redirections
    if (cmd->redirections)
    {
        free_redirections(cmd->redirections);
        cmd->redirections = NULL;
    }

    // Free current_op if it exists
    if (cmd->current_op)
    {
        free(cmd->current_op);
        cmd->current_op = NULL;
    }

    // Add any additional cleanup for other fields in t_command here

    return (1); // Indicate an error occurred
}