/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 14:39:00 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/23 15:35:02 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
# include <sys/stat.h>
# include <signal.h>
# include <sys/wait.h>
# include <errno.h>
# include <libgen.h>
#include <sys/ioctl.h>
# include <limits.h>




# define PROMPT "\001\033[0;32m\002minishell> \001\033[0m\002"
# define MAX_INPUT_LENGTH 4096

typedef struct s_signal {
    volatile sig_atomic_t signal_status;
    int rl_done;
} t_signal;

extern t_signal g_signal;


typedef struct s_expanded_words {
    char *expanded;
    char *original;
} t_expanded_words;

typedef enum e_redirection_type {
    REDIR_INPUT,      // <
    REDIR_OUTPUT,     // >
    REDIR_APPEND,     // >>
    REDIR_HEREDOC     // <<
}   t_redirection_type;

typedef struct s_redirection
{
    t_redirection_type    type;
    int                   redir_number;
    char                  *filename;
    char                  *delimiter;
    char                  *heredoc_file;
	int                   delimiter_quoted;
	struct s_redirection  *next;
}   t_redirection;


typedef struct s_token
{
	char			*value;
	char			*original_value;
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
	int				is_end;
	struct s_token	*next;
}				t_token;

typedef struct s_command
{
	char				*command_string;
	int					index;
	int                 token_index;
	int					is_recalled;
	t_token				*token_list;
	t_redirection       *redirections;
	char                *current_op;
	struct s_command	*next;
}				t_command;

typedef struct s_pid_node {
    pid_t pid;
    struct s_pid_node *next;
} t_pid_node;


typedef struct s_shell_data
{
	volatile sig_atomic_t	signal_status;
	char					*input;
	char					*full_input;
	t_command				*commands;
	t_pid_node 				*pid_list;
	int						pipe_nb;
	int						**pipes;
	int						cmds_nb;
	char					**envp;
	int						exit_status;
	char  					*last_error_file;
	int						error_flag;
	int						in_child_process;
	int						interactive_mode;
	int 					in_single_quote;
    int 					in_double_quote;
    char					 prev_char;
	int 					var_not_found_flag;
	char					*filename_or_delimiter;
	int						in_heredoc;
	int						expanded;
	int						double_quoted;
	char    				*prev_dir;
	char                    *original_word;
	int						saved_stdin;
	int						saved_stdout;
}				t_shell_data;

typedef struct s_parse_context
{
    t_shell_data    *shell;
    char            *input;
    int             *i;
    t_redirection   *redir;
}                   t_parse_context;




void	init_shell(t_shell_data *shell);
int	handle_input(t_shell_data *shell);
int 	check_input_length(t_shell_data *shell);
int 	check_syntax_error(t_shell_data *shell, char *error_message);
int 	read_input(t_shell_data *shell);
int		check_unclosed_quotes(char *input);

int	split_cmd_tokenize(t_shell_data *shell);
int		tokenize_command(t_shell_data *shell, t_command *cmd);
t_command	*create_command(t_shell_data *shell, char *cmd_str, int index);
int		add_token(char *token_value, t_token **token_list, int *index, int is_operator);
void	parse_tokens(t_shell_data *shell);//void	print_commands(t_shell_data *shell);
void	free_commands(t_shell_data *shell);
void 	free_shell_resources(t_shell_data *shell);
void	free_tokens(t_token *token_list);
int 	process_operator(t_shell_data *shell, int *i, t_command *cmd);
int 	process_word(t_shell_data *shell, char *input, int *i, t_command *cmd);


int		process_heredoc_delimiter(t_shell_data *shell ,char *input, int *i,
			t_token *heredoc_token);
int 	read_heredoc_content(t_shell_data *shell, t_redirection *redir);
char	*generate_temp_filename(void);
int		is_operator_char(char c);
int		is_valid_operator(char *op);

char	*getenv_from_envp(t_shell_data *shell, char *name);
int		get_var_name_len(char *str);
char	*expand_variables_in_token(t_shell_data *shell, char *input);

int		ft_strcmp(const char *s1, const char *s2);
char	*ft_strjoin_safe(const char *s1, const char *s2);
char	*ft_strjoin_free_both(char *s1, char *s2);
int		add_char_to_token(char **token, char c);
int		skip_quotes(char *input, int i);

void	handle_tokenization_error(t_shell_data *shell, int error_flag);

int	preprocess_input(t_shell_data *shell);
void	cleanup(t_shell_data *shell);

void	initialize_new_token(t_token *new_token, char *token_value,
			int *index, int is_operator);// 			int *in_double_quote);
void	skip_spaces(t_shell_data *shell, int *i);
int		extract_command_string(char *input, int i);
void	add_command_to_list(t_shell_data *shell, t_command **last_cmd, t_command *cmd);
int		check_delimiter_quotes(t_redirection *redir);
int		heredoc_open_error(char *tmp_filename);
int		expand_and_write_line(t_shell_data *shell, char *line, int fd);
void	free_heredoc_token(t_token *token);
int		tokenize_command_error(t_command *cmd);
void	print_operator_token(t_token *token);
void	print_command_token(t_token *token);
char	*get_variable_value(t_shell_data *shell, char *input, int *i, int *var_not_found_flag);

int		process_input_segment(t_shell_data *shell, int *i, int *cmd_index, t_command **last_cmd);

void	skip_cmd_spaces(char *str, int *i);

int	collect_word(char *input, int *i, char **word, t_shell_data *shell);

int		append_literal_char(char *input, int *i, char **result);



char    **copy_envp(char **envp);
int		check_trailing_pipe(char *input);
int 	check_leading_pipe(char *input);

void	setup_signal_handlers(int type);


int 	**init_pipes(int cmds_nb);

void    execution(t_shell_data *shell);
void 	set_redirection(t_shell_data *shell, t_redirection *redir);
void 	set_pipes(t_shell_data *shell, t_redirection *redir, int cmds_index);
int 	has_redirs(t_redirection *redir, t_redirection_type type);
int 	open_all_files(t_shell_data *shell, t_redirection *redir);
int 	open_input_file(t_shell_data *shell, t_redirection *redir, int fd_in);
int 	open_output_file(t_shell_data *shell, t_redirection *redir, int fd_out);
int 	open_append_file(t_shell_data *shell, t_redirection *redir, int fd_out);
int		execute_internal_commands(t_shell_data *shell, t_command *cmds);


char 	*get_command_path(t_shell_data *shell, t_token *token);
void 	store_pids(t_shell_data *shell, pid_t pid);
int		get_exec_error_code(int err);
void	execute_parent(t_shell_data *shell);

void 	close_all_pipes(int **pipes, int nb_cmds);
void 	free_paths(char **paths);
void	forking(t_shell_data *shell, t_command *cmds);

void 	add_redirection(t_redirection **redirections, t_redirection *new_redir);
int 	handle_redirection(t_shell_data *shell, char *input, int *i, t_command *cmd);
int 	is_redirection_operator(char *op);
void	free_redirections(t_redirection *redirs);
void	write_error(char *exec_name, char *err_message);
void 	clear_pid_list(t_shell_data *shell);

int		validate_operators(t_shell_data *shell);
void	handle_syntax_error(t_shell_data *shell, t_token *token);
void	handle_ctrl_d(t_shell_data *shell);

int 	starts_with_operator_char(char c);
void handle_heredoc(t_shell_data *shell, t_redirection *redir);
void cleanup_heredocs(t_redirection *redir);


void	handle_empty_input(t_shell_data *shell);
int		validate_input_length(t_shell_data *shell);
int		allocate_resources(t_shell_data *shell);
void	add_to_history_if_needed(t_shell_data *shell);
int		check_and_handle_syntax_errors(t_shell_data *shell);
int	process_and_execute_commands(t_shell_data *shell);


int	initialize_word(char **word);
int	should_break(const t_shell_data *shell, char c);
int	handle_unclosed_quotes(t_shell_data *shell, char **word);
int	finalize_word(char **word);
int	process_character(t_shell_data *shell, char c, char **word);

int	free_word_and_return(char *word, int ret);
int	free_original_and_expanded_and_return(char *original_word, char *expanded_word, int ret);
int	save_and_expand_word(t_shell_data *shell, char *word, char **expanded_word, char **original_word);
int	add_token_to_command(t_command *cmd, char *word);
int	set_original_value(t_command *cmd, char *original_word);



t_redirection *create_new_redirection(char *op);
void skip_whitespace(char *input, int *i);
int handle_syntax_error_s(t_shell_data *shell, t_redirection *new_redir, const char *unexpected_token);
int handle_missing_filename_error(t_shell_data *shell, t_redirection *new_redir);

int process_filename_or_delimiter(
    t_shell_data *shell,
    char *input,
    int *i,
    t_redirection *new_redir
);

int handle_heredoc_redirection(
    t_shell_data *shell,
    t_redirection *new_redir,
    char *filename_or_delimiter
);




int initialize_expansion(t_shell_data *shell, char **result);
int	handle_dollar(t_shell_data *shell, char *input, int *i, char **result);

int	toggle_quotes_and_append(t_shell_data *shell, char current_char, \
int *i, char **result);
int	handle_cd_minus(t_shell_data *shell);
int	initialize_conversion(const char **str, int *sign);
void	print_exit_error2(char *format, char *arg, int *exit_status, int code);
int	parse_number(const char *str, long long *num, int sign);
char	**allocate_new_envp(int current_size, t_shell_data *shell);
int	get_name_length(const char *name);
void	replace_env_var(t_shell_data *shell, int index, const char *str);
int	find_env_index(char **envp, const char *key, int key_len);
char	*parse_key(const char *str);
char	*duplicate_new_var(const char *str, t_shell_data *shell);
void	copy_existing_envp(char **new_envp, char **envp, int current_size);
int	collect_and_expand_word(t_shell_data *shell, char *input, \
int *i, char **expanded_word);
int	add_tokens_to_command(t_command *cmd, char **expanded_word_arr);
char	*rm_quotes(char *word);
int	init_and_collect_word(t_parse_context *ctx, char **word);
void	assign_redirection(t_shell_data *shell, char *expanded_word, \
char *original_word);
int validate_expanded_word(t_shell_data *shell, char *expanded_word);
int	collect_and_expand_redirection_word(t_shell_data *shell, \
		t_parse_context *ctx, t_expanded_words *words);
int	handle_pipe_operator(char *op, t_command *cmd);
int	finalize_redirection(t_shell_data *shell, t_redirection *new_redir);
int	handle_greater_operator(t_shell_data *shell, char *input, int *i, \
t_redirection *new_redir);
int	handle_pipe_op(t_shell_data *shell, char *input, int *i, \
t_redirection *new_redir);
char	*collect_operator(t_command *cmd, int *i);
void	handle_quote(t_shell_data *shell, char current_char, int *i);
int	append_str_to_result(char **result, char *str);
void	toggle_quotes_and_skip(t_shell_data *shell, char current_char, int *i);
int	handle_alpha_or_underscore(t_shell_data *shell, char *input, int *i, \
char **result);
int	handle_question_mark(t_shell_data *shell, char **result, int *i);

int	ft_echo(t_shell_data *shell, t_command *cmd);
int	ft_cd(t_shell_data *shell, t_command *cmd);
int	ft_pwd(t_shell_data *shell, t_command *cmd);
int	ft_export(t_shell_data *shell, t_command *cmd);
int	ft_unset(t_shell_data *shell, t_command *cmd);
int	ft_env(t_shell_data *shell, t_command *cmd);
int	ft_exit_shell(t_shell_data *shell, t_command *cmd);

int	is_numeric(const char *str);
int is_valid_identifier(const char *str, int allow_equals);
int	is_internal_command(char *token);
int	handle_chdir_error(t_shell_data *shell, char *expanded_path);
int	handle_memory_error(t_shell_data *shell);
int	handle_no_home(t_shell_data *shell);
int	handle_tilde_path(t_shell_data *shell, char *path);
void	print_exit_error(char *message, int *exit_status, int code);
void	add_to_env(t_shell_data *shell, const char *str);
void	remove_from_env(t_shell_data *shell, const char *name);
void	print_sorted_env(t_shell_data *shell);
int fork_and_execute(t_shell_data *shell, t_command *cmds, t_token *token);
int	execute_command(t_shell_data *shell, t_command *cmd, t_token *token, int ret);
int	ft_exit_child(t_shell_data *shell, t_command *cmd);

#endif
