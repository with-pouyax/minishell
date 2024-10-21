#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <termios.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft/libft.h"
# include "get_next_line.h"

# define PROMPT "minishell> "

// Token structure
typedef struct s_token
{
    char            *value;
    int             index;
    int             is_command;
    int             is_builtin;
    int             is_flag;
    int             is_argument;
    int             is_operator;
    int             is_file;
    int             is_variable_expansion;
    int             is_env_assignment;
    int             is_int;
    int             var_not_found; // New field to indicate variable not found
    struct s_token  *next;
}               t_token;


// Command structure
typedef struct s_command
{
    char            *command_string;
    int             index;
    t_token         *token_list;
    struct s_command *next;
}               t_command;

// Shell state structure
typedef struct s_shell
{
    char    *input;
    int     exit_status;
    t_command *commands;
    char    **envp;         // Environment variables
}               t_shell;

// Function prototypes
void    init_shell(t_shell *shell, char **envp);
void    handle_input(t_shell *shell);
void    listen_for_input(t_shell *shell);
void    cleanup_shell(t_shell *shell);

// Lexing and parsing functions
void    lex_input(t_shell *shell);
void    tokenize_command(t_command *command, t_shell *shell);
void    parse_tokens(t_shell *shell);

// Token handling functions
void    add_token(char *token_value, t_token **token_list, int *index, t_shell *shell);
void    separate_operator(char *token, t_token **token_list, int *index);
int     is_operator(char *token);
int     is_internal_command(char *token);

// Variable expansion functions
char    *expand_variables(char *input, t_shell *shell, int *var_not_found_flag);
char    *getenv_from_envp(char *name, t_shell *shell);
int     get_var_name_len(char *str);
int     is_special_var(char c);

// Quote handling functions
char    *handle_quotes(char *input, int *i);

// Debugging functions
void    print_tokens(t_token *token_list);
void    print_commands(t_shell *shell);

int     check_unclosed_quotes(char *input);


// Expands a single variable token
char    *expand_variable_token(char *input, int *i, t_shell *shell, int *var_not_found_flag);

// Expands variables inside double quotes
char    *expand_double_quotes(char *input, int *i, t_shell *shell, int *var_not_found_flag);

// Returns a literal character as a string
char    *get_literal_char(char *input, int *i);

// Joins two strings and frees the previous allocations
char    *ft_strjoin_free(char *s1, char *s2);

int ft_strcmp(const char *s1, const char *s2);

#endif
