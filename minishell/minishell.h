#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <signal.h>
# include <termios.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft/libft.h"

# define PROMPT "minishell> "

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
    t_command               *commands;
    char                    **envp;
    int                     exit_status;
    int                     error_flag;
}               t_shell_data;

extern t_shell_data g_data;

// Function prototypes
void    init_shell(void);
void    handle_input(void);
void    tokenize_input(void);
int     tokenize_command(t_command *cmd);
t_command   *create_command(char *cmd_str, int index);
int     add_token(char *token_value, t_token **token_list,
                  int *index, int is_operator);
int     is_operator_char(char c);
int     is_operator(char *token);
int     is_internal_command(char *token);
void    parse_tokens(void);
void    print_commands(void);
void    print_tokens(t_token *token_list);
void    free_commands(void);
int     check_unclosed_quotes(char *input);
char    *ft_strjoin_free(char *s1, char *s2);
char    *ft_strdup_free(char *s1);
char    *getenv_from_envp(char *name);
void    expand_variables_in_tokens(void);
int     skip_quotes(char *input, int i);
int     process_operator(char *input, int *i, t_command *cmd, int *index);
int     process_word(char *input, int *i, t_command *cmd, int *index);
int     add_char_to_token(char **token, char c);
int     ft_strcmp(const char *s1, const char *s2);
int     is_valid_operator(char *op);
char    *expand_variables_in_token(char *input, int *var_not_found_flag);
char    *expand_variable_token(char *input, int *i, int *var_not_found_flag);
char    *get_literal_char(char *input, int *i);
int     get_var_name_len(char *str);
char    *ft_strjoin_safe(const char *s1, const char *s2);
void    process_input(void);
int     handle_unclosed_quotes(void);

#endif
