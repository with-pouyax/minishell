#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <termios.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft.h"
# include "get_next_line.h"

# define PROMPT "minishell> "

// Token structure to store individual tokens from the input
typedef struct s_token
{
    char            *value;      // Token value (e.g., command, argument, operator)
    int             index;       // Index of the token in the command
    int             is_command;  // Is this token a command?
    int             is_builtin;  // Is this a built-in command?
    int             is_flag;     // Is this a flag (e.g., -l)?
    int             is_argument; // Is this an argument?
    int             is_operator; // Is this an operator (e.g., <, >)?
    int             is_file;     // Is this a file for redirection?
    int             is_variable_expansion;  // Is this a variable expansion (e.g., $USER)?
    int             is_env_assignment;      // Is this an environment variable assignment (e.g., VAR=value)?
    int             is_int;      // Is this an internal command (e.g., echo, cd)?
    struct s_token  *next;       // Pointer to the next token in the command
}               t_token;

// Command structure to store commands separated by pipes
typedef struct s_command
{
    char            *command_string;    // Raw command string
    int             index;              // Command index in the pipeline
    t_token         *token_list;        // List of tokens for this command
    struct s_command *next;             // Pointer to the next command (if there's a pipe)
}               t_command;

// Shell state structure
typedef struct s_shell
{
    char    *input;         // User input string
    int     exit_status;    // Exit status of the shell
    t_command *commands;    // Linked list of commands
}               t_shell;

// Function prototypes
void    init_shell(t_shell *shell);
void    handle_input(t_shell *shell);
void    listen_for_input(t_shell *shell);
void    cleanup_shell(t_shell *shell);

// Lexing and parsing functions
void    lex_input(t_shell *shell);
void    tokenize_command(t_command *command);
void    parse_tokens(t_shell *shell);

// Debugging functions to print tokens and commands
void    print_tokens(t_token *token_list);    // Function to print tokens (for debugging)
void    print_commands(t_shell *shell);       // Function to print commands and their tokens (for debugging)

int check_unclosed_quotes(char *input);
#endif
