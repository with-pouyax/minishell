#ifndef INTERNAL_COMMANDS_H
# define INTERNAL_COMMANDS_H

// For PATH_MAX
# include <limits.h> 
# include "../minishell.h"

/* Internal Command Functions */
int	ft_echo(t_shell_data *shell, t_command *cmd);
int	ft_cd(t_shell_data *shell, t_command *cmd);
int	ft_pwd(t_shell_data *shell, t_command *cmd);
int	ft_export(t_shell_data *shell, t_command *cmd);
int	ft_unset(t_shell_data *shell, t_command *cmd);
int	ft_env(t_shell_data *shell, t_command *cmd);
int	ft_exit_shell(t_shell_data *shell, t_command *cmd);

/* Helper Functions */
int	is_numeric(const char *str);
int	is_valid_identifier(const char *str);
int	is_internal_command(char *token);
int	handle_chdir_error(t_shell_data *shell, char *expanded_path);
int	handle_memory_error(t_shell_data *shell);
int	handle_no_home(t_shell_data *shell);
int	handle_tilde_path(t_shell_data *shell, char *path);
int	change_to_home(t_shell_data *shell);
void	print_env(t_shell_data *shell);
void	print_exit_error(char *message, int *exit_status, int code);
void	sort_env(char **sorted_envp, int env_size);
void	print_and_free_env(char **sorted_envp);
void	add_to_env(t_shell_data *shell, const char *str);
void	remove_from_env(t_shell_data *shell, const char *name);
void	print_sorted_env(t_shell_data *shell);
int fork_and_execute(t_shell_data *shell, t_command *cmds, t_token *token);
int execute_parent_command(t_shell_data *shell, t_command *cmds, t_token *token);
int is_parent_command(const char *cmd);
int	execute_command(t_shell_data *shell, t_command *cmd, t_token *token, int ret);
#endif
