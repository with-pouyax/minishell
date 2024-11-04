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

typedef struct s_token
{
	char			*value;
	int				index;
	int				is_command;
	int				is_builtin;
	int				is_flag;
	int				is_argument;
	int				is_operator;
	int				is_file;
	int				is_variable_expansion;
	int				is_env_assignment;
	int				is_int;
	int				var_not_found;
	int				wrong_operator;
	int				is_heredoc;
	char			*heredoc_delimiter;
	char			*heredoc_file;
	struct s_token	*next;
}				t_token;

typedef struct s_command
{
	char				*command_string;
	int					index;
	t_token				*token_list;
	struct s_command	*next;
}				t_command;

typedef struct s_shell_data
{
	volatile sig_atomic_t	signal_status;
	char					*input;
	char					*full_input;
	t_command				*commands;
	char					**envp;
	int						exit_status;
	int						error_flag;
}				t_shell_data;

extern t_shell_data	g_data;

/* Function Prototypes */

/* Shell Initialization and Input Handling */
void	init_shell(void);
void	handle_input(void);
void	process_input(void);
int		handle_unclosed_quotes(void);
int		check_unclosed_quotes(char *input);

/* Tokenization, Parsing, and Execution */
void	tokenize_input(void);
int		tokenize_command(t_command *cmd);
t_command	*create_command(char *cmd_str, int index);
int		add_token(char *token_value, t_token **token_list,
			int *index, int is_operator);
void	parse_tokens(void);
void	expand_variables_in_tokens(void);
void	print_commands(void);
void	print_tokens(t_token *token_list);
void	free_commands(void);
void	free_tokens(t_token *token_list);
int		process_operator(char *input, int *i, t_command *cmd,
			int *index);
int		process_word(char *input, int *i, t_command *cmd,
			int *index);

/* Heredoc Handling and Redirection */
int		process_heredoc_delimiter(char *input, int *i,
			t_token *heredoc_token);
int		read_heredoc_content(t_token *heredoc_token);
char	*generate_temp_filename(void);
int		is_operator_char(char c);
int		is_valid_operator(char *op);

/* Environment Variable Handling */
char	*getenv_from_envp(char *name);
int		get_var_name_len(char *str);
char	*expand_variable_token(char *input, int *i,
			int *var_not_found_flag);
char	*expand_variables_in_token(char *input,
			int *var_not_found_flag);
char	*get_literal_char(char *input, int *i);

/* String Manipulation Utilities */
int		ft_strcmp(const char *s1, const char *s2);
char	*ft_strdup_free(char *s1);
char	*ft_strjoin_safe(const char *s1, const char *s2);
char	*ft_strjoin_and_free_first(char *s1, const char *s2);
char	*ft_strjoin_and_free_second(const char *s1, char *s2);
char	*ft_strjoin_and_free_both(char *s1, char *s2);
char	*ft_strjoin_free_both(char *s1, char *s2);
int		add_char_to_token(char **token, char c);
int		skip_quotes(char *input, int i);

/* Error Handling */
void	handle_tokenization_error(int error_flag);

/* Preprocessing */
void	preprocess_input(void);
void	cleanup(void);

/* Additional Helper Functions */
void	initialize_new_token(t_token *new_token, char *token_value,
			int *index, int is_operator);
void	update_quote_flags(char c, int *in_single_quote,
			int *in_double_quote);
int		process_quoted_word(char *input, int *i, char **word);
int		append_additional_input(char *additional_input);
void	skip_spaces(int *i);
int		extract_command_string(char *input, int i);
void	add_command_to_list(t_command **last_cmd, t_command *cmd);
void	tokenize_input_error(int error_flag);
int		process_quoted_delimiter(char *input, int *i);
void	skip_until_operator_or_space(char *input, int *i);
int		syntax_error_newline(void);
int		check_delimiter_quotes(t_token *heredoc_token);
int		heredoc_open_error(char *tmp_filename);
int		handle_heredoc_line(char *line, t_token *heredoc_token,
			int fd, int delimiter_quoted);
int		append_heredoc_full_input(char *line);
int		expand_and_write_line(char *line, int fd);
void	free_heredoc_token(t_token *token);
int		tokenize_command_error(t_command *cmd);
void	print_operator_token(t_token *token);
void	print_command_token(t_token *token);
void	process_operator_details(char *op, t_command *cmd, int *i, int *index);
int		free_and_return(char *str);
char *get_variable_value(char *input, int *i, int *var_not_found_flag);

/* tokenize_input.c */
void	tokenize_input(void);
void	skip_spaces(int *i);
int		process_input_segment(int *i, int *cmd_index, t_command **last_cmd);
int		extract_command_string(char *input, int i);
void	add_command_to_list(t_command **last_cmd, t_command *cmd);

/* tokenize_input_utils.c */
int		tokenize_command(t_command *cmd);
void	skip_cmd_spaces(char *str, int *i);
int		process_token(t_command *cmd, int *i, int *index);
int		tokenize_command_error(t_command *cmd);

/* process_word function */
int		process_word(char *input, int *i, t_command *cmd, int *index);
int		collect_word(char *input, int *i, char **word);
int		process_quoted_word(char *input, int *i, char **word);

/* expand_variables_in_token.c */
char	*expand_variables_in_token(char *input, int *var_not_found_flag);
int		process_variable_expansion(char *input, int *i, char **result, int *flag);
int		append_literal_char(char *input, int *i, char **result);

/* variable_expansion_utils.c */
void	update_quote_flags(char c, int *in_single_quote, int *in_double_quote);
char	*get_variable_value(char *input, int *i, int *var_not_found_flag);

int		get_heredoc_delimiter(char *input, int *i, t_token *heredoc_token);
void	process_heredocs(void);

#endif
