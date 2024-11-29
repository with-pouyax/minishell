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
# include "../libft/libft.h"
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>




# define PROMPT "\001\033[0;32m\002minishell> \001\033[0m\002"
# define MAX_INPUT_LENGTH 4096

typedef enum e_redirection_type {
    REDIR_INPUT,      // <
    REDIR_OUTPUT,     // >
    REDIR_APPEND,     // >>
    REDIR_HEREDOC     // <<
}   t_redirection_type;

typedef struct s_redirection
{
    t_redirection_type    type;               // Type of redirection
    int                   redir_number;       // Redirection number (optional)
    char                  *filename;          // Filename for input/output redirections
    char                  *delimiter;         // Delimiter for heredoc
    char                  *heredoc_file;
	struct s_redirection  *next;              // Pointer to the next redirection (linked list)
}   t_redirection;


typedef struct s_token
{
	char			*value;
	char			*original_value; // New field added
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
	int				is_end; // New field added
	struct s_token	*next;
}				t_token;

typedef struct s_command
{
	char				*command_string;
	int					index;
	int					is_recalled;   //pak shavad
	t_token				*token_list;
	t_redirection       *redirections;   // Add this line
	struct s_command	*next;
}				t_command;

typedef struct s_pid_node {
    pid_t pid;
    struct s_pid_node *next;
} t_pid_node;


typedef struct s_shell_data
{
	volatile sig_atomic_t	signal_status;
	char					*input;            //
	char					*full_input;       //
	t_command				*commands;
	t_pid_node 				*pid_list;           //store process IDs
	int						pipe_nb;
	int						**pipes;
	int						cmds_nb;
	char					**envp;
	int						exit_status;
	int						error_flag;
	int						in_child_process; // Add this line
}				t_shell_data;

#include"internal_cmd/internal_commands.h"

/* Function Prototypes */

/* Shell Initialization and Input Handling */
void	init_shell(t_shell_data *shell);
void	handle_input(t_shell_data *shell);
void 	handle_ctrl_d(void);
int 	check_input_length(t_shell_data *shell);
int 	handle_allocation(t_shell_data *shell);
int 	check_syntax_error(t_shell_data *shell, char *error_message);
int 	read_input(t_shell_data *shell);
void	process_input(t_shell_data *shell);
int		handle_unclosed_quotes(void);
int		check_unclosed_quotes(char *input);

/* Tokenization, Parsing, and Execution */
void	split_cmd_tokenize(t_shell_data *shell);
int		tokenize_command(t_shell_data *shell, t_command *cmd);
t_command	*create_command(t_shell_data *shell, char *cmd_str, int index);
int		add_token(char *token_value, t_token **token_list, int *index, int is_operator);
void	parse_tokens(t_shell_data *shell);
void	expand_variables_in_tokens(t_shell_data *shell);
void	print_commands(t_shell_data *shell);
void	print_tokens(t_token *token_list);
void	free_commands(t_shell_data *shell);
void 	free_shell_resources(t_shell_data *shell);
void	free_tokens(t_token *token_list);
int 	process_operator(t_shell_data *shell, char *input, int *i, t_command *cmd, int *index, int *redir_count);
int 	process_word(t_shell_data *shell, char *input, int *i, t_command *cmd, int *index);


/* Heredoc Handling and Redirection */
int		process_heredoc_delimiter(t_shell_data *shell ,char *input, int *i,
			t_token *heredoc_token);
int 	read_heredoc_content(t_shell_data *shell, t_redirection *redir);
char	*generate_temp_filename(void);
int		is_operator_char(char c);
int		is_valid_operator(char *op);

/* Environment Variable Handling */
char	*getenv_from_envp(t_shell_data *shell, char *name);
int		get_var_name_len(char *str);
char	*expand_variable_token(t_shell_data *shell, char *input, int *i,
int 	*var_not_found_flag);
char	*expand_variables_in_token(t_shell_data *shell, char *input,
int 	*var_not_found_flag);
void 	expand_variables_in_input(t_shell_data *shell);
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
void	handle_tokenization_error(t_shell_data *shell, int error_flag);

/* Preprocessing */
void	preprocess_input(t_shell_data *shell);
void	cleanup(t_shell_data *shell);

/* Additional Helper Functions */
void	initialize_new_token(t_token *new_token, char *token_value,
			int *index, int is_operator);
void	update_quote_flags(char c, int *in_single_quote,
			int *in_double_quote);
int		process_quoted_word(char *input, int *i, char **word);
int		append_additional_input(char *additional_input);
void	skip_spaces(t_shell_data *shell, int *i);
int		extract_command_string(char *input, int i);
void	add_command_to_list(t_shell_data *shell, t_command **last_cmd, t_command *cmd);
void	tokenize_input_error(t_shell_data *shell,int error_flag);
int		process_quoted_delimiter(char *input, int *i);
void	skip_until_operator_or_space(char *input, int *i);
int		syntax_error_newline(void);
int		check_delimiter_quotes(t_redirection *redir);
int		heredoc_open_error(char *tmp_filename);
int 	handle_heredoc_line(t_shell_data *shell, char *line, t_redirection *redir, int fd, int delimiter_quoted);
int		append_heredoc_full_input(char *line);
int		expand_and_write_line(t_shell_data *shell, char *line, int fd);
void	free_heredoc_token(t_token *token);
int		tokenize_command_error(t_command *cmd);
void	print_operator_token(t_token *token);
void	print_command_token(t_token *token);
void	process_operator_details(char *op, t_command *cmd, int *i, int *index);
int		free_and_return(char *str);
char	*get_variable_value(t_shell_data *shell, char *input, int *i, int *var_not_found_flag);

/* tokenize_input.c */
void	tokenize_input(void);
int		process_input_segment(t_shell_data *shell, int *i, int *cmd_index, t_command **last_cmd);
int		extract_command_string(char *input, int i);

/* tokenize_input_utils.c */
int		tokenize_command(t_shell_data *shell, t_command *cmd);
void	skip_cmd_spaces(char *str, int *i);
int process_token(t_shell_data *shell, t_command *cmd, int *i, int *index, int *redir_count);
int		tokenize_command_error(t_command *cmd);

/* process_word function */
int		collect_word(char *input, int *i, char **word);
int		process_quoted_word(char *input, int *i, char **word);

/* expand_variables_in_token.c */
int		process_variable_expansion(t_shell_data *shell, char *input, int *i, char **result, int *flag);
int		append_literal_char(char *input, int *i, char **result);

/* variable_expansion_utils.c */
void	update_quote_flags(char c, int *in_single_quote, int *in_double_quote);

int		get_heredoc_delimiter(char *input, int *i, t_token *heredoc_token);
int		is_recalled_command(char *input);
void	set_recalled_flag(t_command *commands);
char	*get_line_from_input(char *input, int *index);

void    free_envp(t_shell_data *shell);
char    **copy_envp(char **envp);
int		check_trailing_pipe(char *input);
int 	check_leading_pipe(char *input);

// signal handler

void	setup_signal_handlers(void);
void	sigint_handler(int sig);
void	sigquit_handler(int sig);

// execution

int 	calc_cmds_nb(t_shell_data *shell);
int		calc_pipe_nb(t_shell_data *shell);
int 	**init_pipes(int cmds_nb);

void    execution(t_shell_data *shell);
void 	exec_cmd(t_shell_data *shell ,t_command *cmds, int index);
void 	set_redirection(t_shell_data *shell, t_redirection *redir);
void 	set_pipes(t_shell_data *shell, t_redirection *redir, int cmds_index);
int 	has_redirs(t_redirection *redir, t_redirection_type type);
int 	open_all_files(t_shell_data *shell, t_redirection *redir);
int 	open_input_file(t_shell_data *shell, t_redirection *redir, int fd_in);
int 	open_output_file(t_redirection *redir, int fd_out);
int 	open_append_file(t_redirection *redir, int fd_out);
int		execute_internal_commands(t_shell_data *shell, t_command *cmds);
void	execute_external_commands(t_shell_data *shell, t_command *cmds);

// external_execution

char 	*get_command_path(t_shell_data *shell, t_token *token);
char 	*find_path_in_env(t_shell_data *shell, char *cmd);
char	**get_paths_from_env(char **env);
char	*check_and_return_path(const char *cmd, char **all_paths);
void 	store_pids(t_shell_data *shell, pid_t pid);
void 	handle_exec_error(t_shell_data *shell, char *cmd, char *message, int exit_code);
int		get_exec_error_code(int err);
void	quit_program(int exit_code);
void	exec_external_child(t_shell_data *shell, char *cmd_path, char **argv);
void execute_parent(t_shell_data *shell);

char	**convert_tokens_to_argv(t_token *token_list);
int 	token_list_length(t_token *token);
void 	close_all_pipes(int **pipes, int nb_cmds);
void 	close_pipes_after_execution(t_shell_data *shell, int cmds_index);
void 	free_pipes(int **pipes, int nb_cmds);


void    append_end_token(t_shell_data *shell);


void 	add_redirection(t_redirection **redirections, t_redirection *new_redir);
int 	handle_redirection(t_shell_data *shell, char *op, char *input, int *i, t_command *cmd, int *redir_count);
int 	is_redirection_operator(char *op);
void	free_redirections(t_redirection *redirs);
void	write_error(char *exec_name, char *err_message);
void 	clear_pid_list(t_shell_data *shell);

int	validate_operators(t_shell_data *shell);


#endif
