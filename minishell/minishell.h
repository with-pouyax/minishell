#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <string.h>
# include <signal.h>
# include <termios.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft/libft.h"

# define PROMPT "minishell> "

// Structs and Global Variables

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
    int             var_not_found;
    int             wrong_operator;
    int             is_heredoc;           // Indicates if the token is a heredoc operator
    char            *heredoc_delimiter;   // Stores the heredoc delimiter
    char            *heredoc_file;        // Stores the path to the heredoc content file
    struct s_token  *next;
}               t_token;

typedef struct s_command
{
    char                *command_string;
    int                 index;
    t_token             *token_list;
    struct s_command    *next;
}               t_command;

typedef struct s_shell_data
{
    volatile sig_atomic_t    signal_status;
    char                    *input;
    char                    *full_input;    // Full input including heredoc content
    t_command               *commands;
    char                    **envp;
    int                     exit_status;
    int                     error_flag;

    // New fields added
    int                     var_not_found_flag;
    int                     i;                // Index used during parsing
    int                     token_index;      // Token index during tokenization
    char                    *expansion_input; // Input string for variable expansion
}               t_shell_data;

extern t_shell_data g_data;

// Function Prototypes

// Shell Initialization and Input Handling
void    init_shell(void);
void    handle_input(void);
void    process_input(void);
int     handle_unclosed_quotes(void);
int     check_unclosed_quotes(char *input);

// Tokenization, Parsing, and Execution
void    tokenize_input(void);
int     tokenize_command(t_command *cmd);
t_command *create_command(char *cmd_str, int index);
int     add_token(char *token_value, t_token **token_list, int is_operator);
void    parse_tokens(void);
void    expand_variables_in_tokens(void);
void    print_commands(void);
void    print_tokens(t_token *token_list);
void    free_commands(void);
void    free_tokens(t_token *token_list);

// Updated function prototypes for process_operator and process_word
int     process_operator(char *input, t_command *cmd);
int     process_word(char *input, t_command *cmd);

// Updated Heredoc Handling and Redirection function prototypes
int     process_heredoc_delimiter(char *input, t_token *heredoc_token);
int     read_heredoc_content(t_token *heredoc_token);
char    *generate_temp_filename(void);
int     is_operator_char(char c);
int     is_valid_operator(char *op);

// Environment Variable Handling
char    *getenv_from_envp(char *name);
int     get_var_name_len(char *str);
char    *expand_variable_token(void);
char    *expand_variables_in_token(void);
char    *get_literal_char(void);

// String Manipulation Utilities
int     ft_strcmp(const char *s1, const char *s2);
char    *ft_strdup_free(char *s1);
char    *ft_strjoin_safe(const char *s1, const char *s2);
char    *ft_strjoin_and_free_first(char *s1, const char *s2);     // Frees only s1
char    *ft_strjoin_and_free_second(const char *s1, char *s2);    // Frees only s2
char    *ft_strjoin_and_free_both(char *s1, char *s2);            // Frees both s1 and s2
char    *ft_strjoin_free_both(char *s1, char *s2);                // Renamed from ft_strjoin_free
int     add_char_to_token(char **token, char c);
int     skip_quotes(char *input, int i);

// Error Handling
void    handle_tokenization_error(int error_flag);

// Preprocessing
void    preprocess_input(void);
void cleanup(void);


#endif
