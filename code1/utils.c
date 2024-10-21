#include "minishell.h"

// Function to free the commands list
void free_commands(t_command *cmd_list)
{
    t_command *cmd;
    t_token *token;
    t_token *next_token;

    while (cmd_list)
    {
        cmd = cmd_list;
        cmd_list = cmd_list->next;

        token = cmd->token_list;
        while (token)
        {
            next_token = token->next;
            free(token->value);
            free(token);
            token = next_token;
        }

        free(cmd->command_string);
        free(cmd);
    }
}

// Function to get the length of a variable name
int get_var_name_len(char *str)
{
    int len = 0;

    while (str[len] && (ft_isalnum(str[len]) || str[len] == '_'))
        len++;

    return len;
}

// Function to get the environment variable from envp
char *getenv_from_envp(char *name, t_shell *shell)
{
    int i = 0;
    int len = ft_strlen(name);

    while (shell->envp[i])
    {
        if (ft_strncmp(shell->envp[i], name, len) == 0 && shell->envp[i][len] == '=')
            return (shell->envp[i] + len + 1);
        i++;
    }
    return NULL;
}

// Function to check if a variable is special, like "$?"
int is_special_var(char c)
{
    return (c == '?' || ft_isdigit(c));
}
