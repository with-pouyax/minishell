#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <string.h>
# include <fcntl.h>
# include <errno.h>
# include <limits.h>
# include <sys/wait.h>
# include <ctype.h>

// Buffer size for input and other utilities
# define BUFFER_SIZE 1024

// Token types for command parsing
typedef enum e_token_type
{
    WORD,
    PIPE,
    INPUT_REDIRECT,
    OUTPUT_REDIRECT,
    APPEND_REDIRECT,
    HEREDOC
}   t_token_type;

// Structure for a token
typedef struct s_token
{
    char            *value;
    int             type;
    struct s_token  *next;
}   t_token;

// Structure for a command (linked list)
typedef struct s_command
{
    char                *cmd;
    char                **args;
    int                 input_fd;
    int                 output_fd;
    char                *heredoc;
    struct s_command    *next;
}   t_command;

// Global shell structure to store environment, last exit status, and commands
typedef struct s_shell
{
    char        **envp;
    int         last_exit_status;
    t_command   *cmd_list;
}   t_shell;

extern t_shell g_shell;

// Initialization and Cleanup
void    init_shell(char **envp);
void    cleanup_shell(void);

// Main loop
void    shell_loop(void);

// Signal handling
void    signal_setup(void);
void    sigint_handler(int signum);

// Parsing and Execution
int     parse_input(char *input);
void    execute_commands(void);

// Built-in Commands
int     is_builtin(char *cmd);
int     execute_builtin(t_command *cmd);
int     builtin_cd(char **args);
int     builtin_echo(char **args);
int     builtin_pwd(void);
int     builtin_export(char **args);
int     builtin_unset(char **args);
int     builtin_env(void);
int     builtin_exit(char **args);

// Environment Variable Handling
char    *expand_env_vars(char *input);
int     update_env(char *name, char *value);
void    unset_env_var(char *name);
void    print_sorted_env(void);
void    exit_shell(int status);

// Utility Functions
void    free_array(char **array);
void    free_command_list(void);
char    *ft_strdup(const char *s1);
char    *ft_substr(char const *s, unsigned int start, size_t len);
char    *ft_strjoin(char const *s1, char const *s2);
char    *ft_strjoin_free(char *s1, char *s2);
char    *ft_strjoin3(char *s1, char *s2, char *s3);
char    *ft_strjoin_path(char *s1, char *s2);
char    **ft_split(const char *s, char c);
size_t  ft_strlen(const char *s);
int     ft_strcmp(const char *s1, const char *s2);
int     ft_strncmp(const char *s1, const char *s2, size_t n);
int     ft_atoi(const char *str);
int     ft_isdigit(int c);
int     ft_isnumber(char *str);
void    ft_putstr_fd(char *s, int fd);
void    ft_putchar_fd(char c, int fd);
void    ft_putendl_fd(char *s, int fd);
char    *ft_itoa(int n);
int     ft_isalnum(int c);
char    *ft_strchr(const char *s, int c);

// Lexer and Parser
int         tokenize_input(char *input, t_token **tokens);
t_token     *create_token(char *value, int type);
void        add_token(t_token **head, t_token *new_token);
int         is_whitespace(char c);
int         is_special_char(char c);
char        *get_word(char *input, int *i);
char        *get_quoted_word(char *input, int *i);
t_command   *parse_tokens(t_token *tokens);
t_command   *create_command(void);
void        add_command(t_command **head, t_command *new_cmd);
int         handle_redirection(t_token **tokens, t_command *cmd);
int         handle_heredoc(char *delimiter, t_command *cmd);
int         print_error(char *message);
void        free_tokens(t_token *tokens);
void        free_command(t_command *cmd);

// Command Execution
void    execute_commands(void);
void    execute_external(t_command *cmd);
void    child_process(t_command *cmd, int input_fd, int output_fd);
char    *get_command_path(char *cmd);

// History Management
void    load_history(void);
void    save_history(void);
void    add_to_history(char *input);

// Environment Utilities
void    free_envp(void);
char    **copy_envp(void);
void    sort_envp(char **envp);
char    **realloc_envp(char **envp, char *new_var);
int     set_env_var(char *arg);

#endif
